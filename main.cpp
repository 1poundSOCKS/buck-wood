#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <iostream>
#include <tchar.h>

#include "app.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct render_state
{
  bool valid;
  ID2D1SolidColorBrush* brush;
};

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
};

struct control_state
{
  bool quit;
  bool left;
  bool right;
  bool accelerate;
};

std::unique_ptr<render_state> InitRenderState(const app_globals*);
void CleanRenderState(render_state*);

std::unique_ptr<control_state> GetControlState(app_globals*);

std::unique_ptr<game_state> InitGameState();
void UpdateGameState(app_globals*,control_state*,game_state*);

void DoRender(app_globals*,game_state*,perf_data*);

ATOM MyRegisterClass(HINSTANCE hInstance);
void InitInstance(app_globals*);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool ProcessMessage(MSG*);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  std::unique_ptr<app_globals> ag = InitApp(hInstance, nCmdShow);
  if( !ag->initComplete )
  {
    DeinitApp(ag.get());
    return 1;
  }

  std::unique_ptr<game_state> gs = InitGameState();

  MSG msg;

  LARGE_INTEGER perfFrequency;
  QueryPerformanceFrequency(&perfFrequency);

  LARGE_INTEGER initialTicks,ticks,previousTicks;
  ticks.QuadPart = 0;
  
  QueryPerformanceCounter(&initialTicks);
	
  while (ProcessMessage(&msg))
  {
    if( ag->terminating ) continue;

    previousTicks = ticks;
    QueryPerformanceCounter(&ticks);

    perf_data pd;
    pd.totalTicks = ticks.QuadPart - initialTicks.QuadPart;
    pd.frameTicks = ticks.QuadPart - previousTicks.QuadPart;
    pd.fps = pd.frameTicks ? perfFrequency.QuadPart / pd.frameTicks : 0;
    
    std::unique_ptr<control_state> cs = GetControlState(ag.get());
    
    if( cs->quit )
    {
      ::PostQuitMessage(0);
      ag->terminating = true;
      continue;
    }

    UpdateGameState(ag.get(),cs.get(),gs.get());
    DoRender(ag.get(),gs.get(),&pd);
	}

  DeinitApp(ag.get());

  return (int) msg.wParam;
}

void DoRender(app_globals* ag, game_state* gs, perf_data* pd)
{
  if( ag->d2d_rendertarget == NULL ) return;

  std::unique_ptr<render_state> rs = InitRenderState(ag);

  if( !rs->valid )
  {
    CleanRenderState(rs.get());
    ::PostQuitMessage(0);
    ag->terminating = true;
    return;
  }

  D2D1_SIZE_F renderTargetSize = ag->d2d_rendertarget->GetSize();
  ag->d2d_rendertarget->BeginDraw();
  ag->d2d_rendertarget->SetTransform(D2D1::Matrix3x2F::Identity());
  ag->d2d_rendertarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  if( gs == NULL )
  {
    ag->d2d_rendertarget->EndDraw();
    CleanRenderState(rs.get());
    return;
  }

  D2D1_RECT_F rectangle = D2D1::RectF(gs->xPos - 10, gs->yPos - 10, gs->xPos + 10, gs->yPos + 10);
  ag->d2d_rendertarget->FillRectangle(&rectangle, rs->brush);

  WCHAR textMsg[256];
  int msgLen = 0;
  wsprintf(textMsg, L"%i,%i", ag->mouseX, ag->mouseY);
  msgLen = wcslen(textMsg);
  ag->d2d_rendertarget->DrawTextW(textMsg,msgLen,ag->writeTextFormat,D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),rs->brush);

  if( pd )
  {
    _ui64tow(pd->fps,textMsg,10);
    msgLen = wcslen(textMsg);
  }

  ag->d2d_rendertarget->EndDraw();

  CleanRenderState(rs.get());
}

std::unique_ptr<game_state> InitGameState()
{
  std::unique_ptr<game_state> gs = std::make_unique<game_state>();

  gs->xPos = gs->yPos = 0;
  gs->xVelocity = gs->yVelocity = 0;
  
  return gs;
}

void UpdateGameState(app_globals* ag,control_state* cs,game_state* gs)
{
  if( ag->mouseLButtonDown )
  {
    double xDist = static_cast<double>(ag->mouseX) - gs->xPos;
    double xAccel = xDist / 1000.0;
    gs->xVelocity += xAccel;

    double yDist = static_cast<double>(ag->mouseY) - gs->yPos;
    double yAccel = yDist / 1000.0;
    gs->yVelocity += yAccel;
  }

  gs->xPos = gs->xPos + gs->xVelocity;
  gs->yPos = gs->yPos + gs->yVelocity;
}

std::unique_ptr<render_state> InitRenderState(const app_globals *ag)
{
  std::unique_ptr<render_state> rs = std::make_unique<render_state>();
  ::ZeroMemory(rs.get(),sizeof(render_state));

  HRESULT hr = ag->d2d_rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f)), &rs->brush);
  if( FAILED(hr) ) return rs;

  rs->valid = true;
  return rs;
}

void CleanRenderState(render_state* rs)
{
  SafeRelease(rs->brush);
}

std::unique_ptr<control_state> GetControlState(app_globals* ag)
{
  std::unique_ptr<control_state> cs = std::make_unique<control_state>();
  ZeroMemory(cs.get(),sizeof(control_state));

  unsigned char keyboardState[256];
  HRESULT hr = ag->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) hr = ag->keyboard->Acquire();
		else return cs;
    if( FAILED(hr) ) return cs;
	}

  if( keyboardState[DIK_ESCAPE] & 0x80 ) cs->quit = true;

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
