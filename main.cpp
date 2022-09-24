#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>
#include <dwrite.h>

#include "d2d_app.h"
#include "d2d_frame.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define PI 3.14159265

struct perf_data
{
  int64_t totalTicks;
  int64_t frameTicks;
  int64_t fps;
};

struct game_state
{
  double xPos;
  double yPos;
  double xVelocity;
  double yVelocity;
  double shipAngle;
};

struct control_state
{
  bool quit;
  bool left;
  bool right;
  bool accelerate;
  int mouseX, mouseY;
};

std::unique_ptr<control_state> GetControlState(d2d_app*);

std::unique_ptr<game_state> InitGameState(d2d_app*);
void UpdateGameState(control_state*,game_state*);

void DoRender(d2d_frame*,game_state*,perf_data*);

ATOM MyRegisterClass(HINSTANCE hInstance);
void InitInstance(d2d_app*);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool ProcessMessage(MSG*);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance,nCmdShow);

  std::unique_ptr<game_state> gs = InitGameState(app.get());

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

    perf_data pd;
    pd.totalTicks = ticks.QuadPart - initialTicks.QuadPart;
    pd.frameTicks = ticks.QuadPart - previousTicks.QuadPart;
    pd.fps = pd.frameTicks ? perfFrequency.QuadPart / pd.frameTicks : 0;
    
    std::unique_ptr<control_state> cs = GetControlState(app.get());
    
    if( cs->quit )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }

    UpdateGameState(cs.get(),gs.get());
    std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app->d2d_rendertarget);
    DoRender(frame.get(),gs.get(),&pd);
	}

  return (int) msg.wParam;
}

void DoRender(d2d_frame* rs, game_state* gs, perf_data* pd)
{
  D2D1_SIZE_F renderTargetSize = rs->renderTarget->GetSize();
  rs->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  rs->renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  rs->renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(gs->shipAngle,D2D1::Point2F(gs->xPos,gs->yPos)));

  D2D1_RECT_F rectangle = D2D1::RectF(gs->xPos - 10, gs->yPos - 10, gs->xPos + 10, gs->yPos + 10);
  rs->renderTarget->FillRectangle(&rectangle, rs->brush.get());

  rs->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  WCHAR textMsg[256] = L"";
  int msgLen = 0;
  // wsprintf(textMsg, L"%i,%i", cs->mouseX, cs->mouseY);
  msgLen = wcslen(textMsg);
  rs->renderTarget->DrawTextW(textMsg,msgLen,rs->writeTextFormat.get(),D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),rs->brush.get());

  if( pd )
  {
    _ui64tow(pd->fps,textMsg,10);
    msgLen = wcslen(textMsg);
  }

  rs->renderTarget->EndDraw();
}

std::unique_ptr<game_state> InitGameState(d2d_app* ag)
{
  std::unique_ptr<game_state> gs = std::make_unique<game_state>();

  D2D_SIZE_F rtSize = ag->d2d_rendertarget->GetSize();

  gs->xPos = rtSize.width / 2;
  gs->yPos = rtSize.height / 2;
  gs->xVelocity = gs->yVelocity = 0;
  
  return gs;
}

void UpdateGameState(control_state* cs,game_state* gs)
{
  if( cs->left )
  {
    gs->shipAngle -= 1;
  }

  if( cs->right )
  {
    gs->shipAngle += 1;
  }

  if( gs->yVelocity < 1.0 ) gs->yVelocity += 0.05; // gravity

  if( cs->accelerate )
  {
    gs->yVelocity -= 0.1 * cos(gs->shipAngle * PI / 180.0);
    gs->xVelocity += 0.1 * sin(gs->shipAngle * PI / 180.0);
  }

  gs->xPos = gs->xPos + gs->xVelocity;
  gs->yPos = gs->yPos + gs->yVelocity;
}

std::unique_ptr<control_state> GetControlState(d2d_app* ag)
{
  std::unique_ptr<control_state> cs = std::make_unique<control_state>();
  ZeroMemory(cs.get(),sizeof(control_state));

  unsigned char keyboardState[256];
  HRESULT hr = ag->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) ag->keyboard->Acquire();
	}

  if( SUCCEEDED(hr) )
  {
    if( keyboardState[DIK_ESCAPE] & 0x80 ) cs->quit = true;
    if( keyboardState[DIK_Z] & 0x80 ) cs->left = true;
    if( keyboardState[DIK_X] & 0x80 ) cs->right = true;
    if( keyboardState[DIK_SPACE] & 0x80 ) cs->accelerate = true;
  }

  DIMOUSESTATE mouseState;
  hr = ag->mouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mouseState));
  if( FAILED(hr) )
  {
    if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) ag->mouse->Acquire();
  }

  if( SUCCEEDED(hr) )
  {
    cs->mouseX = mouseState.lX;
    cs->mouseY = mouseState.lY;
    if( mouseState.rgbButtons[1] & 0x80 ) cs->accelerate = true;
  }

  return cs;
}
