#include "d2d_app.h"

LPWSTR lpszWndClass = L"d2d app";
LPWSTR lpszTitle = L"d2d app";

ATOM RegisterMainWindowClass(HINSTANCE hInstance);
void CreateMainWindow(d2d_app* ag);

d2d_app::d2d_app(HINSTANCE inst,int cmdShow)
   : terminating(false), inst(inst), cmdShow(cmdShow), wnd(NULL)
{
  RegisterMainWindowClass(inst);

	CreateMainWindow(this);
	if( !wnd ) throw L"error";

	RECT rc;
	GetClientRect(wnd, &rc);

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,d2d_factory.put());
  if( FAILED(hr) ) throw L"error";

	hr = d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(this->wnd,D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)),d2d_rendertarget.put());
  if( FAILED(hr) ) throw L"error";

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_PAINT )
  {
		ValidateRect(hWnd, NULL);
    return 0;
	}

  if( message == WM_SIZE )
  {
    d2d_app *ag = reinterpret_cast<d2d_app *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
    if( ag == NULL || !ag->d2d_rendertarget.get() ) return 0;

    UINT width = LOWORD(lParam);
    UINT height = HIWORD(lParam);
    ag->d2d_rendertarget->Resize(D2D1::SizeU(width, height));
 
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
    d2d_app *ag = NULL;
    pcs = (LPCREATESTRUCT)lParam;
    ag = reinterpret_cast<d2d_app*>(pcs->lpCreateParams);
    ::SetWindowLongPtrW(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(ag));
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

void CreateMainWindow(d2d_app* ag)
{
   ag->wnd = CreateWindowW(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, ag->inst, ag);

   if (!ag->wnd) return;

   ShowWindow(ag->wnd, ag->cmdShow);
   UpdateWindow(ag->wnd);
}
