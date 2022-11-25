#include "pch.h"
#include "screens.h"

bool ProcessMessage()
{
  MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return (msg.message != WM_QUIT);
	}

  return true;
}
