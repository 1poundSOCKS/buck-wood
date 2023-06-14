#include "pch.h"
#include "framework.h"
#include "clock_frequency.h"
#include "diagnostics.h"

framework* framework::m_framework = nullptr;

inline auto GetPercentageTime(int64_t frameTicks, int64_t elapsedTime) -> float
{
  return static_cast<float>(elapsedTime) / static_cast<float>(frameTicks) * 100.0f;
}

auto framework::fullScreen() -> void
{
  m_framework->m_swapChain->SetFullscreenState(TRUE, nullptr);
}

auto framework::toggleFullscreenOnKeypress(int key) -> void
{
  m_framework->ToggleFullscreenOnKeypress(key);
}

auto framework::ToggleFullscreenOnKeypress(int key) -> void
{
  if( m_inputState.keyboardState.data[key] & 0x80 && !(m_inputState.previousKeyboardState.data[key] & 0x80) )
  {
    BOOL fullScreen = FALSE;
    m_swapChain->GetFullscreenState(&fullScreen, nullptr);
    m_swapChain->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
  }
}

auto framework::create(HINSTANCE instance, int cmdShow) -> void
{
  m_framework = new framework(instance, cmdShow);
  m_framework->Init();

  dwrite_factory::create();

  auto diagnosticsBrush = CreateScreenRenderBrush(m_framework->m_renderTarget.get(), D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f));
  auto diagnosticsTextFormat = CreateScreenRenderTextFormat(dwrite_factory::get().get(), L"Verdana", DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
  
  m_framework->m_diagnosticsRenderData = screen_diagnostics_render_data { diagnosticsBrush, diagnosticsTextFormat };

  m_rng.seed(static_cast<unsigned int>(performance_counter::QueryValue()));
}

[[nodiscard]] auto framework:: get() -> framework&
{
  return *m_framework;
}

[[nodiscard]] auto framework::windowData() -> window_data&
{
  return get().m_windowData;
}

[[nodiscard]] auto framework::swapChain() -> winrt::com_ptr<IDXGISwapChain>&
{
  return get().m_swapChain;
}

[[nodiscard]] auto framework::d2dFactory() -> winrt::com_ptr<ID2D1Factory>
{
  return get().m_d2dFactory;  
}

[[nodiscard]] auto framework::renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&
{
  return get().m_renderTarget;
}

[[nodiscard]] auto framework::createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
{
  return ::CreatePathGeometry(get().m_d2dFactory.get());
}

[[nodiscard]] auto framework::directSound() -> winrt::com_ptr<IDirectSound8>&
{
  return get().m_directSound;
}

[[nodiscard]] auto framework::primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&
{
  return get().m_primarySoundBuffer;
}

[[nodiscard]] auto framework::keyboard() -> winrt::com_ptr<IDirectInputDevice8>&
{
  return get().m_keyboard;
}

auto framework::present() -> void
{
  get().m_swapChain->Present(get().m_unlockFrameRate ? 0 : 1, 0);
}

[[nodiscard]] auto framework::fps() -> int
{
  return 60;
}

[[nodiscard]] auto framework::isFrameRateUnlocked() -> bool
{
  return get().m_unlockFrameRate;
}

auto framework::unlockFrameRate() -> void
{
  get().m_unlockFrameRate = true;
}

[[nodiscard]] auto framework::gameSpeedMultiplier() -> float
{
  return get().m_gameSpeedMultiplier;
}

auto framework::setGameSpeedMultiplier(float value) -> void
{
  get().m_gameSpeedMultiplier = value;
}

[[nodiscard]] auto framework::gameUpdateInterval(int64_t ticks) -> float
{
  return static_cast<float>(ticks) / static_cast<float>(clock_frequency::get()) * get().m_gameSpeedMultiplier;
}

[[nodiscard]] auto framework::rng() -> std::mt19937&
{
  return m_rng;
}

auto framework::setDiagnosticsUpdateTime(int64_t ticks) -> void
{
  m_framework->m_diagnosticsUpdateTime = ticks;
}

auto framework::setDiagnosticsRenderTime(int64_t ticks) -> void
{
  m_framework->m_diagnosticsRenderTime = ticks;
}

auto framework::renderDiagnostics() -> void
{
  m_framework->RenderDiagnostics();
}

framework::framework(HINSTANCE instance, int cmdShow) : m_instance(instance), m_cmdShow(cmdShow)
{
}

auto framework::Init() -> void
{
  RegisterMainWindowClass(m_instance);
  m_window = CreateMainWindow(m_instance, m_cmdShow, m_windowData);
  m_swapChain = CreateSwapChain(m_window, framework::fps(), 1);
  m_d2dFactory = CreateD2DFactory();
  m_renderTarget = CreateRenderTarget(m_swapChain.get(), m_d2dFactory.get());

  m_directSound = CreateDirectSound(m_window);

  if( m_directSound )
  {
    m_primarySoundBuffer = CreatePrimarySoundBuffer(m_directSound.get());
  }
  
  m_keyboard = CreateKeyboard(m_instance, m_window);

  HRESULT hr = m_swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) 
    throw std::exception();
}

auto framework::ProcessWindowMessages() -> bool
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

auto framework::AddDiagnosticsTime(std::wstring_view label, int64_t ticks) -> void
{
  auto frameTime = performance_counter::QueryFrequency() / framework::fps();
  m_diagnosticsData.emplace_back(std::format(L"{}: {:.1f}", label, GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
}

auto framework::RenderDiagnostics() -> void
{
  m_frameData.Update();
  FormatDiagnostics(m_inputState, std::back_inserter(m_diagnosticsData));
  m_diagnosticsData.emplace_back(std::format(L"fps: {}", m_frameData.GetFPS()));
  auto frameTime = performance_counter::QueryFrequency() / framework::fps();
  m_diagnosticsData.emplace_back(std::format(L"update time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
  m_diagnosticsData.emplace_back(std::format(L"render time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsRenderTime)));
  RenderDiagnostics(m_diagnosticsData);
  m_diagnosticsData.clear();
}
