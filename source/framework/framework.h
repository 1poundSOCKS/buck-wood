#pragma once

#include "screen_render.h"
#include "screen_input_state.h"
#include "performance_counter.h"
#include "frame_data.h"
#include "render_guard.h"
#include "render.h"
#include "sound.h"
#include "sound_buffer.h"
#include "sound_buffer_player.h"
#include "data_files.h"
#include "collisions.h"
#include "dwrite_factory.h"

struct screen_diagnostics_render_data
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<IDWriteTextFormat> textFormat;
};

class framework
{
public:

  static auto create(HINSTANCE instance, int cmdShow) -> void;
  [[nodiscard]] static auto get() -> framework&;
  [[nodiscard]] static auto windowData() -> window_data&;
  [[nodiscard]] static auto swapChain() -> winrt::com_ptr<IDXGISwapChain>&;
  [[nodiscard]] static auto d2dFactory() -> winrt::com_ptr<ID2D1Factory>;
  [[nodiscard]] static auto renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&;
  [[nodiscard]] static auto createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>;
  [[nodiscard]] static auto directSound() -> winrt::com_ptr<IDirectSound8>&;
  [[nodiscard]] static auto primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&;
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

  static auto addDiagnosticsTime(std::wstring_view label, int64_t ticks) -> void;
  static auto setDiagnosticsUpdateTime(int64_t ticks) -> void;
  static auto setDiagnosticsRenderTime(int64_t ticks) -> void;
  static auto renderDiagnostics() -> void;

  template <typename screen_state_type> static auto openScreen() -> void;
  [[nodiscard]] inline static auto screenInputState() -> const screen_input_state&;

private:

  framework(HINSTANCE instance, int cmdShow);
  auto Init() -> void;

  static framework* m_framework;

  template <typename screen_state_type> auto OpenScreen() -> void;
  auto RenderDiagnostics() -> void;
  auto RenderDiagnostics(std::ranges::input_range auto&& objects) -> void;
  auto ToggleFullscreenOnKeypress(int key) -> void;

  auto ProcessWindowMessages() -> bool;

  auto AddDiagnosticsTime(std::wstring_view label, int64_t ticks) -> void;

  static inline std::mt19937 m_rng; // pseudo-random generator

  HINSTANCE m_instance = nullptr;
  int m_cmdShow { 0 };
  HWND m_window = nullptr;
  window_data m_windowData;
  bool m_closeApp { false };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  winrt::com_ptr<ID2D1Factory> m_d2dFactory;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1PathGeometry> m_pathGeometry;
  winrt::com_ptr<IDirectSound8> m_directSound;
  winrt::com_ptr<IDirectSoundBuffer> m_primarySoundBuffer;
  winrt::com_ptr<IDirectInputDevice8> m_keyboard;
  screen_diagnostics_render_data m_diagnosticsRenderData;
  bool m_unlockFrameRate { false };
  float m_gameSpeedMultiplier { 1.0f };
  frame_data m_frameData;
  std::vector<std::wstring> m_diagnosticsData;
  int64_t m_diagnosticsUpdateTime { 0 };
  int64_t m_diagnosticsRenderTime { 0 };
  screen_input_state m_inputState;

};

auto framework::RenderDiagnostics(std::ranges::input_range auto&& objects) -> void
{
  auto diagnosticsString = std::reduce(std::cbegin(objects), std::cend(objects), std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });

  RenderText(m_renderTarget.get(), m_diagnosticsRenderData.brush.get(), m_diagnosticsRenderData.textFormat.get(), diagnosticsString);
}

template <typename screen_state_type> static auto framework::openScreen() -> void
{
  m_framework->OpenScreen<screen_state_type>();
}

template <typename screen_state_type> auto framework::OpenScreen() -> void
{
  screen_state_type screenState;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;

  auto keepScreenOpen { true };
  
  while( !ProcessWindowMessages() && keepScreenOpen )
  {
    m_inputState.windowData = framework::windowData();
    m_inputState.renderTargetMouseData = GetRenderTargetMouseData(m_inputState.windowData, framework::renderTarget().get());

    ReadKeyboardState(framework::keyboard().get(), m_inputState.keyboardState);

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / framework::fps();

    keepScreenOpen = screenState.Refresh(framework::isFrameRateUnlocked() ? currentTime - previousTime : frameTime);

    previousTime = currentTime;
    currentTime = performance_counter::QueryValue();

    m_inputState.previousWindowData = m_inputState.windowData;
    m_inputState.previousKeyboardState = m_inputState.keyboardState;
    m_inputState.previousRenderTargetMouseData = m_inputState.renderTargetMouseData;
  }
}

[[nodiscard]] inline auto framework::screenInputState() -> const screen_input_state&
{
  return m_framework->m_inputState;
}

inline auto framework::addDiagnosticsTime(std::wstring_view label, int64_t ticks) -> void
{
  m_framework->AddDiagnosticsTime(label, ticks);
}
