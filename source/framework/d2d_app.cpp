#include "d2d_app.h"

LPWSTR lpszWndClass = L"d2d app";
LPWSTR lpszTitle = L"d2d app";

ATOM RegisterMainWindowClass(HINSTANCE hInstance);
void CreateMainWindow(d2d_app* app);
winrt::com_ptr<IDirectSoundBuffer> CreatePrimarySoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound);

d2d_app::d2d_app(HINSTANCE inst,int cmdShow, int fps)
   : inst(inst), cmdShow(cmdShow), wnd(NULL), windowWidth(0), windowHeight(0)
{
  RegisterMainWindowClass(inst);

	CreateMainWindow(this);
	if( !wnd ) throw L"error";

  timer = std::make_unique<system_timer>();
  
  perfData = std::make_unique<perf_data>();

  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = 1920;
  swapChainDesc.BufferDesc.Height = 1080;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = fps;
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

  HRESULT hr = S_OK;
  hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, dxgi_swapChain.put(), d3d_device.put(), NULL, NULL);
  if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  hr = dxgi_swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());
  if( FAILED(hr) ) throw L"error";

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

  hr = DirectInput8Create(inst, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetCooperativeLevel(wnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  keyboard->Acquire();

#ifdef USE_DIRECTINPUT_MOUSE
  hr = directInput->CreateDevice(GUID_SysMouse, mouse.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = mouse->SetCooperativeLevel(wnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  DIDATAFORMAT DIDataFormat = c_dfDIMouse;
  DIDataFormat.dwFlags = DIDF_RELAXIS;
  hr = mouse->SetDataFormat(&DIDataFormat);
  if( FAILED(hr) ) throw L"error";
#endif

  hr = DirectSoundCreate8(NULL, directSound.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = directSound->SetCooperativeLevel(wnd, DSSCL_PRIORITY);
  if( FAILED(hr) ) throw L"error";

  primarySoundBuffer = CreatePrimarySoundBuffer(directSound);
}

d2d_app::~d2d_app()
{
  if( dxgi_swapChain ) dxgi_swapChain->SetFullscreenState(FALSE, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_MOUSEMOVE )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    LPARAM mouseX = GET_X_LPARAM(lParam);
    LPARAM mouseY = GET_Y_LPARAM(lParam);
    app->clientMouseX = mouseX;
    app->clientMouseY = mouseY;
    return 0;
  }

  if( message == WM_LBUTTONDOWN )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    app->leftMouseButtonDown = true;
    return 0;
  }

  if( message == WM_LBUTTONUP )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    app->leftMouseButtonDown = false;
    return 0;
  }

  if( message == WM_RBUTTONDOWN )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    app->rightMouseButtonDown = true;
    return 0;
  }

  if( message == WM_RBUTTONUP )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    app->rightMouseButtonDown = false;
    return 0;
  }

  if( message == WM_PAINT )
  {
		ValidateRect(hWnd, NULL);
    return 0;
	}

  if( message == WM_SIZE )
  {
    d2d_app *app = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( app == NULL ) return 0;

    app->windowWidth = LOWORD(lParam);
    app->windowHeight = HIWORD(lParam);

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

  if( message == WM_SETCURSOR )
  {
    SetCursor(NULL);
    return TRUE;
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
    wcex.hCursor        = NULL;
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

winrt::com_ptr<IDirectSoundBuffer> CreatePrimarySoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound)
{
  DSBUFFERDESC bufferDesc;
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  winrt::com_ptr<IDirectSoundBuffer> primaryBuffer;
  HRESULT hr = directSound->CreateSoundBuffer(&bufferDesc, primaryBuffer.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	hr = primaryBuffer->SetFormat(&waveFormat);
  if( FAILED(hr) ) throw L"error";
  
  return primaryBuffer;
}
