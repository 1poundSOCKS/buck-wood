#include "pch.h"
#include "render_target.h"
#include "diagnostics.h"
#include "screen_render.h"

render_target* render_target::m_instance = nullptr;

inline auto GetPercentageTime(int64_t frameTicks, int64_t elapsedTime) -> float
{
  return static_cast<float>(elapsedTime) / static_cast<float>(frameTicks) * 100.0f;
}

auto render_target::ToggleFullscreenOnKeypress(int key) -> void
{
  m_toggleFullscreenKey = key;
}

auto render_target::create(HINSTANCE appInstance, int cmdShow) -> HWND
{
  m_instance = new render_target(appInstance, cmdShow);
  m_instance->Init();

  dwrite_factory::create();

  auto diagnosticsBrush = CreateScreenRenderBrush(m_instance->m_renderTarget.get(), D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f));
  auto diagnosticsTextFormat = CreateScreenRenderTextFormat(dwrite_factory::get().get(), L"Verdana", DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
  
  m_instance->m_diagnosticsRenderData = screen_diagnostics_render_data { diagnosticsBrush, diagnosticsTextFormat };

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

auto render_target::AddDiagnosticsTime(std::wstring_view label, int64_t ticks) -> void
{
  auto frameTime = performance_counter::QueryFrequency() / render_target::fps();
  m_diagnosticsData.emplace_back(std::format(L"{}: {:.1f}", label, GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
}

auto render_target::RenderDiagnostics() -> void
{
  m_frameData.Update();
  FormatDiagnostics(m_inputState, std::back_inserter(m_diagnosticsData));
  m_diagnosticsData.emplace_back(std::format(L"fps: {}", m_frameData.GetFPS()));
  auto frameTime = performance_counter::QueryFrequency() / render_target::fps();
  m_diagnosticsData.emplace_back(std::format(L"update time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
  m_diagnosticsData.emplace_back(std::format(L"render time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsRenderTime)));
  RenderDiagnostics(m_diagnosticsData);
  m_diagnosticsData.clear();
}

auto render_target::RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, std::wstring_view text,
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment) -> void
{
  D2D1_SIZE_F rect = m_renderTarget->GetSize();
  RenderText(brush, textFormat, text, D2D1_RECT_F { 0, 0, rect.width - 1, rect.height - 1 }, paragraphAlignment, textAlignment);
}

auto render_target::RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, std::wstring_view text, const D2D1_RECT_F& rect,
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment) -> void
{
  HRESULT hr1 = textFormat->SetParagraphAlignment(paragraphAlignment);
  HRESULT hr2 = textFormat->SetTextAlignment(textAlignment);

  if( SUCCEEDED(hr1) && SUCCEEDED(hr2) )
  {
    m_renderTarget->DrawText(text.data(), static_cast<UINT32>(text.length()), textFormat, rect, brush);
  }
}
