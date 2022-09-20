#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dinput.h>
#include <memory>

#define SafeRelease(i) if( i ) i->Release()

struct app_globals
{
  bool initComplete;
  bool terminating;
  HINSTANCE inst;
  int cmdShow;
  HWND wnd;
  ID2D1Factory* d2d_factory;
  ID2D1HwndRenderTarget* d2d_rendertarget;
  IDWriteFactory* writeFactory;
  IDWriteTextFormat* writeTextFormat;
  IDirectInput8* directInput;
  LPDIRECTINPUTDEVICE8 keyboard;
  int mouseX, mouseY;
  bool mouseLButtonDown;
};

ATOM MyRegisterClass(HINSTANCE hInstance);
void InitInstance(app_globals* ag);
bool ProcessMessage(MSG* msg);
std::unique_ptr<app_globals> InitApp(HINSTANCE,int);
void DeinitApp(app_globals*);
