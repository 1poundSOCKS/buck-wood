#include "d2d_app.h"

LPWSTR lpszWndClass = L"d2d app";
LPWSTR lpszTitle = L"d2d app";

ATOM RegisterMainWindowClass(HINSTANCE hInstance);
void CreateMainWindow(d2d_app* app);

d2d_app::d2d_app(HINSTANCE inst,int cmdShow)
   : terminating(false), inst(inst), cmdShow(cmdShow), wnd(NULL)
{
  RegisterMainWindowClass(inst);

	CreateMainWindow(this);
	if( !wnd ) throw L"error";

	RECT rc;
	GetClientRect(wnd, &rc);

  HRESULT hr = S_OK;

  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Width = rc.right - rc.left;
  swapChainDesc.BufferDesc.Height = rc.bottom - rc.top;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = wnd;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = TRUE;

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };

  hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, dxgi_swapChain.put(), d3d_device.put(), NULL, NULL);
  if( FAILED(hr) ) throw L"error";

  hr = dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) throw L"error";

  // hr = swapChain->Present(1, 0);
  // if( FAILED(hr) ) throw L"error";

  // winrt::com_ptr<ID3D11Texture2D> renderTarget;
  // hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), renderTarget.put_void());
  // if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  hr = dxgi_swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());
  if( FAILED(hr) ) throw L"error";

  // winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
  // hr = d3dDevice->CreateRenderTargetView(renderTarget.get(), NULL, renderTargetView.put());
  // if( FAILED(hr) ) throw L"error";

  // dxgi_device.as(d3d_device);
  hr = d3d_device->QueryInterface(dxgi_device.put());
  if( FAILED(hr) ) throw L"error";

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
  );
  
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,d2d_factory.put());
  if( FAILED(hr) ) throw L"error";

  hr = d2d_factory->CreateDxgiSurfaceRenderTarget(dxgi_surface.get(), props, d2d_rendertarget.put());
  if( FAILED(hr) ) throw L"error";

	// hr = d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
  // D2D1::HwndRenderTargetProperties(this->wnd,D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)),d2d_rendertarget.put());
  // if( FAILED(hr) ) throw L"error";

  hr = DirectInput8Create(inst, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetCooperativeLevel(wnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->Acquire();
  if( FAILED(hr) ) throw L"error";

  hr = directInput->CreateDevice(GUID_SysMouse, mouse.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = mouse->SetCooperativeLevel(wnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  DIDATAFORMAT DIDataFormat = c_dfDIMouse;
  DIDataFormat.dwFlags = DIDF_RELAXIS;
  hr = mouse->SetDataFormat(&DIDataFormat);
  if( FAILED(hr) ) throw L"error";
}

d2d_app::~d2d_app()
{
  if( dxgi_swapChain ) dxgi_swapChain->SetFullscreenState(FALSE, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_PAINT )
  {
		ValidateRect(hWnd, NULL);
    return 0;
	}

  if( message == WM_SIZE )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL || !app->d2d_rendertarget.get() ) return 0;

    UINT width = LOWORD(lParam);
    UINT height = HIWORD(lParam);
    // app->d2d_rendertarget->Resize(D2D1::SizeU(width, height));
 
    return 0;
  }

  if( message == WM_DISPLAYCHANGE )
  {
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  }

  if( message == WM_CREATE )
  {
    LPCREATESTRUCT pcs = NULL;
    d2d_app *app = NULL;
    pcs = (LPCREATESTRUCT)lParam;
    app = reinterpret_cast<d2d_app*>(pcs->lpCreateParams);
    ::SetWindowLongPtrW(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(app));
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  if( message == WM_DESTROY )
  {
    ::PostQuitMessage(0);
  }

	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM RegisterMainWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = lpszWndClass;
    wcex.hIconSm        = NULL;

    return RegisterClassExW(&wcex);
}

void CreateMainWindow(d2d_app* app)
{
   app->wnd = CreateWindowW(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, app->inst, app);

   if (!app->wnd) return;

   ShowWindow(app->wnd, app->cmdShow);
   UpdateWindow(app->wnd);
}
