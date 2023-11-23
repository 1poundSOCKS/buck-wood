#pragma once

#include "screen_input_state.h"
#include "performance_counter.h"
#include "frame_data.h"
#include "render_guard.h"
#include "sound.h"
#include "sound_buffer.h"
#include "dwrite_factory.h"
#include "clock_frequency.h"
#include "screen_transform.h"

class render_target
{
public:

  static auto create(HINSTANCE appInstance, int cmdShow) -> HWND;
  static auto destroy() -> void;

  [[nodiscard]] static auto get() -> render_target&;
  [[nodiscard]] static auto windowData() -> window_data&;
  [[nodiscard]] static auto swapChain() -> winrt::com_ptr<IDXGISwapChain>&;
  [[nodiscard]] static auto d2dFactory() -> winrt::com_ptr<ID2D1Factory>;
  [[nodiscard]] static auto renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&;
  [[nodiscard]] static auto createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>;
  static auto present() -> void;
  [[nodiscard]] static auto fps() -> int;
  [[nodiscard]] static auto isFrameRateUnlocked() -> bool;
  static auto unlockFrameRate() -> void;
  [[nodiscard]] static auto gameSpeedMultiplier() -> float;
  static auto setGameSpeedMultiplier(float value) -> void;
  [[nodiscard]] static auto gameUpdateInterval(int64_t ticks) -> float;
  [[nodiscard]] static auto rng() -> std::mt19937&;

  static auto fullScreen() -> void;
  static auto toggleFullscreenOnKeypress(int key) -> void;

  template <typename screen_state_type> static auto openScreen() -> void;

  static auto renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  static auto renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  render_target(HINSTANCE appInstance, int cmdShow);
  auto Init() -> void;

  template <typename screen_state_type> auto OpenScreen() -> void;
  auto ToggleFullscreenOnKeypress(int key) -> void;

  auto ProcessWindowMessages() -> bool;

  auto RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  auto RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  static render_target* m_instance;
  static inline std::mt19937 m_rng; // pseudo-random generator

  HINSTANCE m_appInstance = nullptr;
  int m_cmdShow { 0 };
  HWND m_window = nullptr;
  window_data m_windowData;
  bool m_closeApp { false };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  winrt::com_ptr<ID2D1Factory> m_d2dFactory;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1PathGeometry> m_pathGeometry;
  bool m_unlockFrameRate { false };
  float m_gameSpeedMultiplier { 1.0f };
  std::optional<int> m_toggleFullscreenKey;

};

[[nodiscard]] inline auto render_target:: get() -> render_target&
{
  return *m_instance;
}

[[nodiscard]] inline auto render_target::windowData() -> window_data&
{
  return m_instance->m_windowData;
}

[[nodiscard]] inline auto render_target::swapChain() -> winrt::com_ptr<IDXGISwapChain>&
{
  return m_instance->m_swapChain;
}

[[nodiscard]] inline auto render_target::d2dFactory() -> winrt::com_ptr<ID2D1Factory>
{
  return m_instance->m_d2dFactory;  
}

[[nodiscard]] inline auto render_target::renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&
{
  return m_instance->m_renderTarget;
}

[[nodiscard]] inline auto render_target::createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
{
  return ::CreatePathGeometry(m_instance->m_d2dFactory.get());
}

inline auto render_target::present() -> void
{
  m_instance->m_swapChain->Present(m_instance->m_unlockFrameRate ? 0 : 1, 0);
}

[[nodiscard]] inline auto render_target::fps() -> int
{
  return 60;
}

[[nodiscard]] inline auto render_target::isFrameRateUnlocked() -> bool
{
  return m_instance->m_unlockFrameRate;
}

inline auto render_target::unlockFrameRate() -> void
{
  m_instance->m_unlockFrameRate = true;
}

[[nodiscard]] inline auto render_target::gameSpeedMultiplier() -> float
{
  return m_instance->m_gameSpeedMultiplier;
}

inline auto render_target::setGameSpeedMultiplier(float value) -> void
{
  m_instance->m_gameSpeedMultiplier = value;
}

[[nodiscard]] inline auto render_target::gameUpdateInterval(int64_t ticks) -> float
{
  return static_cast<float>(ticks) / static_cast<float>(clock_frequency::get()) * m_instance->m_gameSpeedMultiplier;
}

[[nodiscard]] inline auto render_target::rng() -> std::mt19937&
{
  return m_rng;
}

inline auto render_target::fullScreen() -> void
{
  m_instance->m_swapChain->SetFullscreenState(TRUE, nullptr);
}

inline auto render_target::toggleFullscreenOnKeypress(int key) -> void
{
  m_instance->ToggleFullscreenOnKeypress(key);
}

template <typename screen_state_type> static auto render_target::openScreen() -> void
{
  m_instance->OpenScreen<screen_state_type>();
}

template <typename screen_state_type> auto render_target::OpenScreen() -> void
{
  screen_state_type screenState;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;

  auto keepScreenOpen { true };
  
  while( !ProcessWindowMessages() && keepScreenOpen )
  {
    screen_input_state::update(m_windowData, GetRenderTargetMouseData(m_windowData, m_renderTarget.get()));

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / render_target::fps();

    if( m_toggleFullscreenKey && screen_input_state::keyboardReader().Pressed(*m_toggleFullscreenKey) )
    {
      BOOL fullScreen = FALSE;
      m_swapChain->GetFullscreenState(&fullScreen, nullptr);
      m_swapChain->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
    }

    keepScreenOpen = screenState.Refresh(render_target::isFrameRateUnlocked() ? currentTime - previousTime : frameTime);

    previousTime = currentTime;
    currentTime = performance_counter::QueryValue();

    m_swapChain->Present(m_unlockFrameRate ? 0 : 1, 0);
  }
}

inline auto render_target::renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(brush, textFormat, text);
}

inline auto render_target::renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(rect, brush, textFormat, text);
}
