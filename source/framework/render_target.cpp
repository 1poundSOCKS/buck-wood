#include "pch.h"
#include "render_target.h"
#include "diagnostics.h"
#include "screen_render.h"

render_target* render_target::m_instance = nullptr;

auto render_target::ToggleFullscreenOnKeypress(int key) -> void
{
  m_toggleFullscreenKey = key;
}

auto render_target::create(HINSTANCE appInstance, int cmdShow) -> HWND
{
  m_instance = new render_target(appInstance, cmdShow);
  m_instance->Init();

  m_rng.seed(static_cast<unsigned int>(performance_counter::QueryValue()));

  return m_instance->m_window;
}

auto render_target::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

render_target::render_target(HINSTANCE appInstance, int cmdShow) : m_appInstance { appInstance }, m_cmdShow { cmdShow }
{
}

auto render_target::Init() -> void
{
  RegisterMainWindowClass(m_appInstance);
  m_window = CreateMainWindow(m_appInstance, m_cmdShow, m_windowData);
  m_swapChain = CreateSwapChain(m_window, render_target::fps(), 1);
  m_d2dFactory = CreateD2DFactory();
  m_renderTarget = CreateRenderTarget(m_swapChain.get(), m_d2dFactory.get());

  m_keyboard = CreateKeyboard(m_appInstance, m_window);

  HRESULT hr = m_swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) 
    throw std::exception();
}

auto render_target::ProcessWindowMessages() -> bool
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
      m_closeApp = true;
    }
  }

  return m_closeApp;
}

auto render_target::RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_renderTarget->DrawText(text.data(), static_cast<UINT32>(text.length()), textFormat, rect, brush);
}

auto render_target::RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  D2D1_SIZE_F rect = m_renderTarget->GetSize();
  RenderText(D2D1_RECT_F { 0, 0, rect.width - 1, rect.height - 1 }, brush, textFormat, text);
}
