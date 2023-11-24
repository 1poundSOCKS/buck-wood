#pragma once

#include "performance_counter.h"
#include "user_input.h"

class windows_message_loop
{

public:

  static auto create(const winrt::com_ptr<IDXGISwapChain>& swapChain, int fps) -> void;
  static auto destroy() -> void;

  template <typename screen_state_type> static auto openScreen(const keyboard_reader& keyboardReader, bool unlockFramerate, std::optional<int> toggleFullscreenKey) -> void;

private:

  windows_message_loop(const winrt::com_ptr<IDXGISwapChain>& swapChain, int fps);
  template <typename screen_state_type> auto OpenScreen(const keyboard_reader& keyboardReader, bool unlockFramerate, std::optional<int> toggleFullscreenKey) -> void;
  auto ProcessWindowMessages() -> bool;
  auto ToggleFullscreenOnKeypress(int key) -> void;

private:

  inline static windows_message_loop* m_instance { nullptr };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  int m_fps { 0 };
  bool m_closeApp { false };

};

inline auto windows_message_loop::create(const winrt::com_ptr<IDXGISwapChain>& swapChain, int fps) -> void
{
  destroy();
  m_instance = new windows_message_loop(swapChain, fps);
}

inline auto windows_message_loop::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

inline windows_message_loop::windows_message_loop(const winrt::com_ptr<IDXGISwapChain>& swapChain, int fps) : m_swapChain { swapChain }, m_fps { fps }
{
}

template <typename screen_state_type>
static auto windows_message_loop::openScreen(const keyboard_reader& keyboardReader, bool unlockFramerate, std::optional<int> toggleFullscreenKey) -> void
{
  m_instance->OpenScreen<screen_state_type>(keyboardReader, unlockFramerate, toggleFullscreenKey);
}

template <typename screen_state_type>
auto windows_message_loop::OpenScreen(const keyboard_reader& keyboardReader, bool unlockFramerate, std::optional<int> toggleFullscreenKey) -> void
{
  screen_state_type screenState;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;

  auto keepScreenOpen { true };
  
  while( !ProcessWindowMessages() && keepScreenOpen )
  {
    user_input::update();

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / m_fps;

    if( toggleFullscreenKey && keyboardReader.Pressed(*toggleFullscreenKey) )
    {
      BOOL fullScreen = FALSE;
      m_swapChain->GetFullscreenState(&fullScreen, nullptr);
      m_swapChain->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
    }

    keepScreenOpen = screenState.Refresh(unlockFramerate ? currentTime - previousTime : frameTime);

    previousTime = currentTime;
    currentTime = performance_counter::QueryValue();

    m_swapChain->Present(unlockFramerate ? 0 : 1, 0);
  }
}
