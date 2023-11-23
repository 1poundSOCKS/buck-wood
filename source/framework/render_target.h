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

  static auto create(HWND wnd) -> void;
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
  // [[nodiscard]] static auto gameSpeedMultiplier() -> float;
  // static auto setGameSpeedMultiplier(float value) -> void;
  // [[nodiscard]] static auto gameUpdateInterval(int64_t ticks) -> float;

  static auto fullScreen() -> void;
  static auto toggleFullscreenOnKeypress(int key) -> void;

  template <typename screen_state_type> static auto openScreen(const keyboard_reader& keyboardReader) -> void;

  static auto renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  static auto renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  render_target(HWND wnd);

  template <typename screen_state_type> auto OpenScreen(const keyboard_reader& keyboardReader) -> void;
  auto ToggleFullscreenOnKeypress(int key) -> void;

  auto ProcessWindowMessages() -> bool;

  auto RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  auto RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

  static winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
  static winrt::com_ptr<ID2D1Factory> CreateD2DFactory();
  static winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);
  static winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory);

private:

  static render_target* m_instance;
  HINSTANCE m_appInstance = nullptr;
  bool m_closeApp { false };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  winrt::com_ptr<ID2D1Factory> m_d2dFactory;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  bool m_unlockFrameRate { false };
  float m_gameSpeedMultiplier { 1.0f };
  std::optional<int> m_toggleFullscreenKey;

};

[[nodiscard]] inline auto render_target:: get() -> render_target&
{
  return *m_instance;
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
  return CreatePathGeometry(m_instance->m_d2dFactory.get());
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

// [[nodiscard]] inline auto render_target::gameSpeedMultiplier() -> float
// {
//   return m_instance->m_gameSpeedMultiplier;
// }

// inline auto render_target::setGameSpeedMultiplier(float value) -> void
// {
//   m_instance->m_gameSpeedMultiplier = value;
// }

// [[nodiscard]] inline auto render_target::gameUpdateInterval(int64_t ticks) -> float
// {
//   return static_cast<float>(ticks) / static_cast<float>(clock_frequency::get()) * m_instance->m_gameSpeedMultiplier;
// }

inline auto render_target::fullScreen() -> void
{
  m_instance->m_swapChain->SetFullscreenState(TRUE, nullptr);
}

inline auto render_target::toggleFullscreenOnKeypress(int key) -> void
{
  m_instance->ToggleFullscreenOnKeypress(key);
}

template <typename screen_state_type> static auto render_target::openScreen(const keyboard_reader& keyboardReader) -> void
{
  m_instance->OpenScreen<screen_state_type>(keyboardReader);
}

template <typename screen_state_type> auto render_target::OpenScreen(const keyboard_reader& keyboardReader) -> void
{
  screen_state_type screenState;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;

  auto keepScreenOpen { true };
  
  auto renderTargetSize = m_renderTarget->GetSize();
  
  while( !ProcessWindowMessages() && keepScreenOpen )
  {
    screen_input_state::update();

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / render_target::fps();

    if( m_toggleFullscreenKey && keyboardReader.Pressed(*m_toggleFullscreenKey) )
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
