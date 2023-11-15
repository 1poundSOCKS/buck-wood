#ifndef _main_window_
#define _main_window_

// #include "render.h"
#include "render_target_mouse_data.h"

struct window_mouse_data
{
  int x = 0;
  int y = 0;
  bool leftButtonDown = false;
  bool rightButtonDown = false;
};

struct window_data
{
  WORD width = 0;
  WORD height = 0;
  RECT clientRect = {0, 0, 0, 0};
  window_mouse_data mouse;
};

HWND CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data);
ATOM RegisterMainWindowClass(HINSTANCE instance);
winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
winrt::com_ptr<ID2D1Factory> CreateD2DFactory();
winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);
winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory);
render_target_mouse_data GetRenderTargetMouseData(const window_data& windowData, ID2D1RenderTarget* renderTarget);


#endif
