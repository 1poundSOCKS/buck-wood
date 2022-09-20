#include "app.h"

LPWSTR lpszWndClass = L"buck wood";
LPWSTR lpszTitle = L"buck wood";


std::unique_ptr<app_globals> InitApp(HINSTANCE instance, int nCmdShow)
{
  std::unique_ptr<app_globals> ag = std::make_unique<app_globals>();
  ::ZeroMemory(ag.get(), sizeof(app_globals));
  ag->inst = instance;
  ag->cmdShow = nCmdShow;

  MyRegisterClass(ag->inst);

	InitInstance(ag.get());
	if( !ag->wnd ) return ag;

	RECT rc;
	GetClientRect(ag->wnd, &rc);

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&ag->d2d_factory);
  if( FAILED(hr) ) return ag;

	hr = ag->d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(ag->wnd,D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)),&ag->d2d_rendertarget);
  if( FAILED(hr) ) return ag;

  hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(ag->writeFactory),reinterpret_cast<IUnknown **>(&ag->writeFactory));
  if( FAILED(hr) ) return ag;

  hr = ag->writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,10,L"", &ag->writeTextFormat);
  if( FAILED(hr) ) return ag;

  ag->writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  ag->writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

  hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(&ag->directInput), NULL);
  if( FAILED(hr) ) return ag;

  hr = ag->directInput->CreateDevice(GUID_SysKeyboard, &ag->keyboard, NULL);
  if( FAILED(hr) ) return ag;

  hr = ag->keyboard->SetDataFormat(&c_dfDIKeyboard);
  if( FAILED(hr) ) return ag;

  hr = ag->keyboard->SetCooperativeLevel(ag->wnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if( FAILED(hr) ) return ag;

  hr = ag->keyboard->Acquire();
  if( FAILED(hr) ) return ag;
  
  ag->initComplete = true;

  return ag;
}

void DeinitApp(app_globals *ag)
{
  SafeRelease(ag->keyboard);
  SafeRelease(ag->directInput);
  SafeRelease(ag->writeTextFormat);
  SafeRelease(ag->writeFactory);
  SafeRelease(ag->d2d_rendertarget);
  SafeRelease(ag->d2d_factory);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_MOUSEMOVE )
  {
    app_globals *ag = reinterpret_cast<app_globals *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    ag->mouseX = GET_X_LPARAM(lParam);
    ag->mouseY = GET_Y_LPARAM(lParam);
    return 0;
  }

  if( message == WM_LBUTTONDOWN )
  {
    app_globals *ag = reinterpret_cast<app_globals *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    ag->mouseLButtonDown = true;
    return 0;
  }

  if( message == WM_LBUTTONUP )
  {
    app_globals *ag = reinterpret_cast<app_globals *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    ag->mouseLButtonDown = false;
    return 0;
  }

  if( message == WM_PAINT )
  {
    app_globals *ag = reinterpret_cast<app_globals *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
		ValidateRect(hWnd, NULL);
    return 0;
	}

  if( message == WM_SIZE )
  {
    app_globals *ag = reinterpret_cast<app_globals *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    UINT width = LOWORD(lParam);
    UINT height = HIWORD(lParam);
    if( ag->d2d_rendertarget ) ag->d2d_rendertarget->Resize(D2D1::SizeU(width, height));
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
    app_globals *ag = NULL;
    pcs = (LPCREATESTRUCT)lParam;
    ag = reinterpret_cast<app_globals*>(pcs->lpCreateParams);
    ::SetWindowLongPtrW(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(ag));
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  if( message == WM_DESTROY )
  {
    ::PostQuitMessage(0);
  }

	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
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

void InitInstance(app_globals* ag)
{
   ag->wnd = CreateWindowW(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, ag->inst, ag);

   if (!ag->wnd) return;

   ShowWindow(ag->wnd, ag->cmdShow);
   UpdateWindow(ag->wnd);
}
