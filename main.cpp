#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <d2d1.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"d2d1")

struct app_globals {
  HINSTANCE inst;
  HWND wnd;
  ID2D1Factory* d2d_factory;
  ID2D1HwndRenderTarget* d2d_rendertarget;
};

LPWSTR lpszWndClass = L"cpp_test";
LPWSTR lpszTitle = L"cpp_test";

app_globals InitApp(HINSTANCE,int);
void DeinitApp(app_globals);
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define SafeRelease(i) if( i ) i->Release()

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  app_globals ag = InitApp(hInstance, nCmdShow);

  ag.d2d_rendertarget->BeginDraw();

	ag.d2d_rendertarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	ID2D1SolidColorBrush* brush = NULL;
  HRESULT hr = ag.d2d_rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 0.0f, 0.5f, 1.0f)), &brush);
  if( SUCCEEDED(hr) )
  {
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, 100, 100);
    ag.d2d_rendertarget->FillRectangle(&rectangle, brush);
    SafeRelease(brush);
  }

  ag.d2d_rendertarget->EndDraw();

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
  {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

  DeinitApp(ag);

  return (int) msg.wParam;
}

app_globals InitApp(HINSTANCE instance, int nCmdShow)
{
  app_globals ag;
  ::ZeroMemory(&ag, sizeof(app_globals));
  ag.inst = instance;

  MyRegisterClass(ag.inst);

	ag.wnd = InitInstance(ag.inst, nCmdShow);
	if( !ag.wnd ) return ag;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&ag.d2d_factory);

  if( FAILED(hr) ) return ag;

	RECT rc;
	GetClientRect(ag.wnd, &rc);

	hr = ag.d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(ag.wnd,D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)),
	  &ag.d2d_rendertarget);

  return ag;
}

void DeinitApp(app_globals ag)
{
  SafeRelease(ag.d2d_rendertarget);
  SafeRelease(ag.d2d_factory);
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

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd = CreateWindowW(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return NULL;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_COMMAND:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;

	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
	}
	break;

	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
	{
		ValidateRect(hWnd, NULL);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)
		{
    }
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }
  
  return DefWindowProc(hWnd, message, wParam, lParam);
}
