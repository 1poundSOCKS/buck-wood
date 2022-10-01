#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>

#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "math.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")

std::unique_ptr<control_state> GetControlState(const std::unique_ptr<d2d_app>& app, const std::unique_ptr<control_state>& previousControlState);
void UpdateGameState(const std::unique_ptr<control_state>&, std::unique_ptr<game_state>&,double timespanSeconds);
void DrawGameObject(const game_object&, winrt::com_ptr<ID2D1HwndRenderTarget>,winrt::com_ptr<ID2D1SolidColorBrush>);
void SetTransformAndDrawGameObject(const game_object&, winrt::com_ptr<ID2D1HwndRenderTarget>,winrt::com_ptr<ID2D1SolidColorBrush>);
bool ProcessMessage(MSG* msg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance,nCmdShow);

  D2D_SIZE_F rtSize = app->d2d_rendertarget->GetSize();
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>(static_cast<int>(rtSize.width / 2), static_cast<int>(rtSize.height / 2));

  MSG msg;

  LARGE_INTEGER perfFrequency;
  QueryPerformanceFrequency(&perfFrequency);

  LARGE_INTEGER initialTicks,ticks,previousTicks;
  ticks.QuadPart = 0;
  
  QueryPerformanceCounter(&initialTicks);
	
  std::unique_ptr<control_state> previousControlState = std::make_unique<control_state>();

  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    previousTicks = ticks;
    QueryPerformanceCounter(&ticks);

    const std::unique_ptr<perf_data> perfData = std::make_unique<perf_data>(perfFrequency,initialTicks,ticks,previousTicks);
    
    std::unique_ptr<control_state> controlState = GetControlState(app, previousControlState);
    
    UpdateGameState(controlState,gameState,perfData->frameTimeSeconds);

    D2D1_SIZE_F frameSize = app->d2d_rendertarget->GetSize();
    const int gameScreenWidth = 1000;
    const int gameScreenHeight = 500;
    const double scaleX = static_cast<double>(frameSize.width) / static_cast<double>(gameScreenWidth);
    const double scaleY = static_cast<double>(frameSize.height) / static_cast<double>(gameScreenHeight);
    gameState->cursor.xPos = controlState->mouseX / scaleX;
    gameState->cursor.yPos = controlState->mouseY / scaleY;

    previousControlState = std::move(controlState);

    if( !gameState->running )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }

    std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app->d2d_rendertarget,scaleX,scaleY);
    DoRender(frame,gameState,perfData);
	}

  return (int) msg.wParam;
}

void UpdateGameState(const std::unique_ptr<control_state>& cs, std::unique_ptr<game_state>& gs, double timespanSeconds)
{
  if( cs->quitPress )
  {
    switch( gs->screen )
    {
      case game_state::title:
        gs->running = false;
        return;
      case game_state::main:
        gs->screen = game_state::title;
        break;
    }
  }

  if( gs->screen == game_state::title )
  {
    if( cs->startGame )
    {
      gs->screen = game_state::main;
    }

    return;
  }

  if( cs->left ) gs->player.angle -= 2;
  if( cs->right ) gs->player.angle += 2;
  
  if( gs->player.yVelocity < 2.0 ) gs->player.yVelocity += ( timespanSeconds * 5 ); // gravity

  if( cs->shoot )
  {
    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
    newBullet->gameObject.xPos = gs->player.xPos;
    newBullet->gameObject.yPos = gs->player.yPos;
    double angle = CalculateAngle(gs->player.xPos, gs->player.yPos, gs->cursor.xPos, gs->cursor.yPos);
    newBullet->gameObject.angle = angle;
    newBullet->gameObject.Accelerate(timespanSeconds * 500.0);
    gs->bullets.push_front(std::move(newBullet));
  }

  if( cs->accelerate ) gs->player.Accelerate(timespanSeconds * 10.0);

  gs->Update(timespanSeconds);
}

std::unique_ptr<control_state> GetControlState(const std::unique_ptr<d2d_app>& app, const std::unique_ptr<control_state>& previousControlState)
{
  std::unique_ptr<control_state> cs = std::make_unique<control_state>();

  unsigned char keyboardState[256];
  HRESULT hr = app->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) app->keyboard->Acquire();
	}

  if( SUCCEEDED(hr) )
  {
    if( keyboardState[DIK_ESCAPE] & 0x80 )
    {
      cs->quit = true;
      if( !previousControlState->quit ) cs->quitPress = true;
    }
    if( keyboardState[DIK_SPACE] & 0x80 ) cs->startGame = true;
    if( keyboardState[DIK_Z] & 0x80 ) cs->left = true;
    if( keyboardState[DIK_X] & 0x80 ) cs->right = true;
    if( keyboardState[DIK_SPACE] & 0x80 ) cs->accelerate = true;
  }

  DIMOUSESTATE mouseState;
  hr = app->mouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mouseState));
  if( FAILED(hr) )
  {
    if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) app->mouse->Acquire();
  }

  if( SUCCEEDED(hr) )
  {
    POINT p;
    if( GetCursorPos(&p) )
    {
      POINT clientPos;
      if( ScreenToClient(app->wnd, &p) )
      {
        cs->mouseX = p.x;
        cs->mouseY = p.y;
      }
    }
    if( mouseState.rgbButtons[0] & 0x80 ) cs->shoot = true;
    if( mouseState.rgbButtons[1] & 0x80 ) cs->accelerate = true;
  }

  return cs;
}

bool ProcessMessage(MSG* msg)
{
	if (PeekMessage(msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg->hwnd, NULL, msg))
    {
      TranslateMessage(msg);
      DispatchMessage(msg);
    }
    return (msg->message != WM_QUIT);
	}

  return true;
}
