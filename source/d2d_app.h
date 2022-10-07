#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dxgi.h>
#include <dinput.h>
#include <memory>
#include <winrt/base.h>

struct d2d_app
{
  d2d_app(HINSTANCE inst,int cmdShow);

  bool terminating;
  HINSTANCE inst;
  int cmdShow;
  HWND wnd;
  winrt::com_ptr<ID2D1Factory> d2d_factory;
  winrt::com_ptr<ID2D1HwndRenderTarget> d2d_rendertarget;
  winrt::com_ptr<IDirectInput8> directInput;
  winrt::com_ptr<IDirectInputDevice8> keyboard;
  winrt::com_ptr<IDirectInputDevice8> mouse;
};
