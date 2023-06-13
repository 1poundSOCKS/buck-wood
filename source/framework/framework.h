#pragma once

#include "screen_render.h"
#include "screen_input_state.h"
#include "performance_counter.h"
#include "perf_data.h"
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
  [[nodiscard]] static auto CreatePathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>;
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

  static auto renderDiagnostics(std::ranges::input_range auto&& objects) -> void;

private:

  framework(HINSTANCE instance, int cmdShow);
  auto Init() -> void;
  static framework* m_framework;

  static inline std::mt19937 m_rng; // pseudo-random generator

  HINSTANCE m_instance = nullptr;
  int m_cmdShow { 0 };
  HWND m_window = nullptr;
  window_data m_windowData;
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
};

auto framework::renderDiagnostics(std::ranges::input_range auto&& objects) -> void
{  
  auto diagnosticsString = std::reduce(std::cbegin(objects), std::cend(objects), std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });

  RenderText(get().m_renderTarget.get(), get().m_diagnosticsRenderData.brush.get(), get().m_diagnosticsRenderData.textFormat.get(), diagnosticsString);
}
