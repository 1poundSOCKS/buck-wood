#include "pch.h"
#include "windows_message_loop.h"

windows_message_loop::windows_message_loop(const winrt::com_ptr<IDXGISwapChain>& swapChain, std::optional<int> fps, std::optional<int> toggleFullscreenKey) : 
  m_swapChain { swapChain }, m_fps { fps }, m_toggleFullscreenKey { toggleFullscreenKey }
{
  m_presentOnVsync = m_fps ? 1 : 0;
}

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
