#include "pch.h"
#include "main_window.h"

LPCWSTR lpszWndClass = L"main_window";
LPCWSTR lpszTitle = L"main_window";

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterMainWindowClass(HINSTANCE instance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = instance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = NULL;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = lpszWndClass;
    wcex.hIconSm        = NULL;
    
    return RegisterClassExW(&wcex);
}

HWND CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data)
{
  HWND windowHandle = ::CreateWindow(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, &data);
  if( !windowHandle ) throw L"error";

  ShowWindow(windowHandle, cmdShow);
  UpdateWindow(windowHandle);
  return windowHandle;
}

winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator)
{
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = 1920;
  swapChainDesc.BufferDesc.Height = 1080;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRateNumerator;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateDenominator;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = window;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = TRUE;

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };

  winrt::com_ptr<ID3D11Device> d3dDevice;
  winrt::com_ptr<IDXGISwapChain> swapChain;

  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
    featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, swapChain.put(), d3dDevice.put(), NULL, NULL);

  if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDXGIDevice> dxgiDevice; // NOT USED: gets thrown away
  hr = d3dDevice->QueryInterface(dxgiDevice.put());
  if( FAILED(hr) ) throw L"error";

  return swapChain;
}

winrt::com_ptr<ID2D1Factory> CreateD2DFactory()
{
  winrt::com_ptr<ID2D1Factory> d2dFactory;
	
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,d2dFactory.put());
  if( FAILED(hr) ) throw L"error";
  
  return d2dFactory;
}

winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory)
{
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  HRESULT hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());
  if( FAILED(hr) ) throw L"error";

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
  );
  
  hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgi_surface.get(), props, renderTarget.put());
  if( FAILED(hr) ) throw L"error";

  return renderTarget;
}

winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory)
{
  winrt::com_ptr<ID2D1PathGeometry> pathGeometry;

  HRESULT hr = d2dFactory->CreatePathGeometry(pathGeometry.put());
  if( FAILED(hr) ) throw L"error";

  return pathGeometry;
}

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_MOUSEMOVE )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->mouse.x = GET_X_LPARAM(lParam);
    data->mouse.y = GET_Y_LPARAM(lParam);
    return 0;
  }

  if( message == WM_LBUTTONDOWN )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->mouse.leftButtonDown = true;
    return 0;
  }

  if( message == WM_LBUTTONUP )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->mouse.leftButtonDown = false;
    return 0;
  }

  if( message == WM_RBUTTONDOWN )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->mouse.rightButtonDown = true;
    return 0;
  }

  if( message == WM_RBUTTONUP )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->mouse.rightButtonDown = false;
    return 0;
  }

  if( message == WM_PAINT )
  {
		ValidateRect(handle, NULL);
    return 0;
	}

  if( message == WM_SIZE )
  {
    window_data *data = reinterpret_cast<window_data*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(handle,GWLP_USERDATA)));
    if( data == NULL ) return 0;

    data->width = LOWORD(lParam);
    data->height = HIWORD(lParam);
    GetClientRect(handle, &data->clientRect);
    return 0;
  }

  if( message == WM_DISPLAYCHANGE )
  {
    InvalidateRect(handle, NULL, FALSE);
    return 0;
  }

  if( message == WM_CREATE )
  {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    window_data *data = reinterpret_cast<window_data*>(pcs->lpCreateParams);
    ::SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
    return DefWindowProc(handle, message, wParam, lParam);
  }

  if( message == WM_DESTROY )
  {
    ::PostQuitMessage(0);
  }

  if( message == WM_SETCURSOR )
  {
    SetCursor(NULL);
    return TRUE;
  }

	return DefWindowProc(handle, message, wParam, lParam);
}
