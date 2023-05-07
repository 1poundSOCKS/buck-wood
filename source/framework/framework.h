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
#include "screen_runner.h"
#include "dwrite_factory.h"

class framework
{
public:

  static auto create(HINSTANCE instance, int cmdShow) -> void;
  static auto get() -> framework&;
  static auto windowData() -> window_data&;
  static auto swapChain() -> winrt::com_ptr<IDXGISwapChain>&;
  static auto renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&;
  static auto directSound() -> winrt::com_ptr<IDirectSound8>&;
  static auto primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&;
  static auto keyboard() -> winrt::com_ptr<IDirectInputDevice8>&;
  static auto fps() -> int;
  static auto rng() -> std::mt19937&;

  template <typename screen_state> static auto openScreen() -> void;
  static auto fullScreen() -> void;

private:

  framework(HINSTANCE instance, int cmdShow);
  auto Init() -> void;
  static framework* m_framework;

  static inline std::mt19937 m_rng; // pseudo-random generator

  HINSTANCE m_instance = nullptr;
  int m_cmdShow = 0;
  HWND m_window = nullptr;
  window_data m_windowData;
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDirectSound8> m_directSound;
  winrt::com_ptr<IDirectSoundBuffer> m_primarySoundBuffer;
  winrt::com_ptr<IDirectInputDevice8> m_keyboard;
};

template <typename screen_state> static auto framework::openScreen() -> void
{
  screen_state screenState;

  screen_runner_data screenRunnerData
  {
    swapChain(),
    renderTarget(), 
    dwrite_factory::get(),
    keyboard(), 
    windowData(),
    fps()
  };

  OpenScreen(screenRunnerData, screenState);
}
