#include "pch.h"
#include "windows_message_loop.h"

auto windows_message_loop::ProcessWindowMessages() -> bool
{
  MSG msg;

  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  
    if( msg.message == WM_QUIT )
    {
      m_continue = false;
    }
  }

  return m_continue;
}
