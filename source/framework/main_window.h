#ifndef _main_window_
#define _main_window_

#include "window_data.h"
#include "render_target_mouse_data.h"

HWND CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data);
ATOM RegisterMainWindowClass(HINSTANCE instance);
winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
winrt::com_ptr<ID2D1Factory> CreateD2DFactory();
winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);
winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory);

#endif
