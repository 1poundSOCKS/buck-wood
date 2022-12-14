#ifndef _main_window_
#define _main_window_

#include "render.h"

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

struct render_target_mouse_data
{
  float x = 0;
  float y = 0;
  D2D1_SIZE_F size;
};

HWND CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data);
ATOM RegisterMainWindowClass(HINSTANCE instance);
winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain);
render_target_mouse_data GetRenderTargetMouseData(const window_data& windowData, ID2D1RenderTarget* renderTarget);

#endif
