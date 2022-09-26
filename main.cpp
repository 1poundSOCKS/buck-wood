#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>

#include "d2d_app.h"
#include "d2d_frame.h"
#include "game_state.h"
#include "control_state.h"
#include "perf_data.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define PI 3.14159265

std::unique_ptr<control_state> GetControlState(const std::unique_ptr<d2d_app>&);
void UpdateGameState(const std::unique_ptr<control_state>&, std::unique_ptr<game_state>&);
void DoRender(const std::unique_ptr<d2d_frame>&, const std::unique_ptr<game_state>&, const std::unique_ptr<perf_data>&);
bool ProcessMessage(MSG* msg);
void DrawGameObject(const game_object&, winrt::com_ptr<ID2D1HwndRenderTarget>,winrt::com_ptr<ID2D1SolidColorBrush>);

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
	
  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    previousTicks = ticks;
    QueryPerformanceCounter(&ticks);

    const std::unique_ptr<perf_data> perfData = std::make_unique<perf_data>(perfFrequency,initialTicks,ticks,previousTicks);
    
    std::unique_ptr<control_state> controlState = GetControlState(app);
    
    if( controlState->quit )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }

    UpdateGameState(controlState,gameState);

    std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app->d2d_rendertarget);
    
    DoRender(frame,gameState,perfData);
	}

  return (int) msg.wParam;
}

void DoRender(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  D2D1_SIZE_F renderTargetSize = frame->renderTarget->GetSize();
  frame->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame->renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  DrawGameObject(gs->player, frame->renderTarget, frame->brush);

  frame->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  WCHAR textMsg[256] = L"D2D Demo";
  int msgLen = 0;
  wsprintf(textMsg, L"%i,%i", static_cast<int>(gs->player.xPos), static_cast<int>(gs->player.yPos));
  msgLen = wcslen(textMsg);
  frame->renderTarget->DrawTextW(textMsg,msgLen,frame->writeTextFormat.get(),D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),frame->brush.get());

  _ui64tow(pd->fps,textMsg,10);
  msgLen = wcslen(textMsg);
}

void DrawGameObject(const game_object& gameObject, winrt::com_ptr<ID2D1HwndRenderTarget> renderTarget, winrt::com_ptr<ID2D1SolidColorBrush> brush)
{
  D2D1_RECT_F rectangle = gameObject.GetRectangleForRender();
  renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(gameObject.xPos,gameObject.yPos)));
  renderTarget->FillRectangle(&rectangle, brush.get());
}

void UpdateGameState(const std::unique_ptr<control_state>& cs, std::unique_ptr<game_state>& gs)
{
  if( !gs->started && !cs->accelerate ) return;

  gs->started = true;

  if( cs->left )
  {
    gs->player.angle -= 2;
  }

  if( cs->right )
  {
    gs->player.angle += 2;
  }

  if( gs->player.yVelocity < 2.0 ) gs->player.yVelocity += 0.05; // gravity

  if( cs->accelerate )
  {
    gs->player.yVelocity -= 0.1 * cos(gs->player.angle * PI / 180.0);
    gs->player.xVelocity += 0.1 * sin(gs->player.angle * PI / 180.0);
  }

  gs->player.UpdatePosition();
}

std::unique_ptr<control_state> GetControlState(const std::unique_ptr<d2d_app>& app)
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
    if( keyboardState[DIK_ESCAPE] & 0x80 ) cs->quit = true;
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
    cs->mouseX = mouseState.lX;
    cs->mouseY = mouseState.lY;
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
