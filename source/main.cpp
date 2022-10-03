#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>

#include "math.h"
#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "game_level.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")

std::unique_ptr<control_state> GetControlState(const std::unique_ptr<d2d_app>& app, const std::unique_ptr<control_state>& previousControlState);
void UpdateGameState(const std::unique_ptr<control_state>&, std::unique_ptr<game_state>&,float timespanSeconds);
bool ProcessMessage(MSG* msg);
std::unique_ptr<game_state> CreateGameState();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance,nCmdShow);

  std::unique_ptr<game_state> gameState = CreateGameState();

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
    DoRender(app->d2d_rendertarget, gameState, perfData, controlState->mouseX, controlState->mouseY);
    UpdateGameState(controlState, gameState, perfData->frameTimeSeconds);
    previousControlState = std::move(controlState);

    if( !gameState->running )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }
	}

  return (int) msg.wParam;
}

std::unique_ptr<game_state> CreateGameState()
{
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>();
  gameState->currentLevel = CreateGameLevel();
  gameState->player.xPos = gameState->currentLevel->width / 2.0f;
  gameState->player.yPos = gameState->currentLevel->height / 2.0f;
  return gameState;
}

void UpdateGameState(const std::unique_ptr<control_state>& cs, std::unique_ptr<game_state>& gs, float timespanSeconds)
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
    if( cs->startGame ) gs->screen = game_state::main;
    return;
  }

  if( cs->left ) gs->player.spin = -100.0f;
  else if( cs->right ) gs->player.spin = 100.0f;
  else gs->player.spin = 0.0f;
  
  static const float forceOfGravity = 10.0f;
  static const float maxPlayerFallVelocity = 300.0f;
  static const float playerThrust = 20.0f;

  gs->player.yVelocity += ( forceOfGravity );
  if( gs->player.yVelocity > maxPlayerFallVelocity ) gs->player.yVelocity = maxPlayerFallVelocity;
  if( cs->shoot ) gs->OnPlayerShoot();
  if( cs->accelerate ) gs->player.Accelerate(playerThrust);
  
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
