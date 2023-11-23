#include "pch.h"
#include "main_window.h"

LPCWSTR lpszWndClass = L"main_window";
LPCWSTR lpszTitle = L"main_window";

main_window* main_window::m_instance { nullptr };

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

ATOM main_window::RegisterMainWindowClass(HINSTANCE instance)
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

HWND main_window::CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data)
{
  HWND windowHandle = ::CreateWindow(lpszWndClass, lpszTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, &data);
  if( !windowHandle ) throw L"error";

  ShowWindow(windowHandle, cmdShow);
  UpdateWindow(windowHandle);
  return windowHandle;
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
