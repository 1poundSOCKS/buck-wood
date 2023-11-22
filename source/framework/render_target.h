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
  [[nodiscard]] static auto keyboard() -> winrt::com_ptr<IDirectInputDevice8>&;
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
  // [[nodiscard]] static auto screenInputState() -> const screen_input_state&;
  static auto DisableMouse() -> void;
  [[nodiscard]] static auto MouseEnabled() -> bool;

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
  winrt::com_ptr<IDirectInputDevice8> m_keyboard;
  bool m_unlockFrameRate { false };
  float m_gameSpeedMultiplier { 1.0f };
  std::optional<int> m_toggleFullscreenKey;
  // screen_input_state m_inputState;
  bool m_mouseEnabled { true };

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

[[nodiscard]] inline auto render_target::keyboard() -> winrt::com_ptr<IDirectInputDevice8>&
{
  return m_instance->m_keyboard;
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
    // m_inputState.windowData = render_target::windowData();
    // m_inputState.renderTargetMouseData = GetRenderTargetMouseData(m_inputState.windowData, render_target::renderTarget().get());

    // m_inputState.keyboardReader.Update(render_target::keyboard().get());
    // m_inputState.gamepadReader.Update();

    screen_input_state::update(render_target::windowData(), 
      GetRenderTargetMouseData(screen_input_state::windowData(), render_target::renderTarget().get()),
      render_target::keyboard().get());

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / render_target::fps();

    // if( m_toggleFullscreenKey && m_inputState.keyboardReader.Pressed(*m_toggleFullscreenKey) )
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

    // m_inputState.previousWindowData = m_inputState.windowData;
    // m_inputState.previousRenderTargetMouseData = m_inputState.renderTargetMouseData;
    screen_input_state::next();
  }
}

// [[nodiscard]] inline auto render_target::screenInputState() -> const screen_input_state&
// {
//   return m_instance->m_inputState;
// }

inline auto render_target::DisableMouse() -> void
{
  m_instance->m_mouseEnabled = false;
}

[[nodiscard]] inline auto render_target::MouseEnabled() -> bool
{
  return m_instance->m_mouseEnabled;
}

inline auto render_target::renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(brush, textFormat, text);
}

inline auto render_target::renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(rect, brush, textFormat, text);
}
