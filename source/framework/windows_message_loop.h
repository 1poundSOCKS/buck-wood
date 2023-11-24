#pragma once

#include "performance_counter.h"
#include "user_input.h"

class windows_message_loop
{

public:

  static auto create(const winrt::com_ptr<IDXGISwapChain>& swapChain, std::optional<int> fps, std::optional<int> toggleFullscreenKey) -> void;
  static auto destroy() -> void;

  static auto run(auto& callable) -> void;

private:

  windows_message_loop(const winrt::com_ptr<IDXGISwapChain>& swapChain, std::optional<int> fps, std::optional<int> toggleFullscreenKey);
  template <typename screen_state_type> auto OpenScreen(const keyboard_reader& keyboardReader) -> void;
  template <typename screen_state_type> auto RefreshScreen(screen_state_type& screenState, __int64 frameTime, const keyboard_reader& keyboardReader) -> bool;
  auto ProcessWindowMessages() -> bool;
  auto ToggleFullscreenOnKeypress(int key) -> void;

private:

  inline static windows_message_loop* m_instance { nullptr };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  std::optional<int> m_fps;
  std::optional<int> m_toggleFullscreenKey;
  bool m_presentOnVsync { true };
  bool m_closeApp { false };

};

inline auto windows_message_loop::create(const winrt::com_ptr<IDXGISwapChain>& swapChain, std::optional<int> fps, std::optional<int> toggleFullscreenKey) -> void
{
  destroy();
  m_instance = new windows_message_loop(swapChain, fps, toggleFullscreenKey);
}

inline auto windows_message_loop::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

inline windows_message_loop::windows_message_loop(const winrt::com_ptr<IDXGISwapChain>& swapChain, std::optional<int> fps, std::optional<int> toggleFullscreenKey) : 
  m_swapChain { swapChain }, m_fps { fps }, m_toggleFullscreenKey { toggleFullscreenKey }
{
  m_presentOnVsync = m_fps ? 1 : 0;
}

inline auto windows_message_loop::run(auto& callable) -> void
{
  while( !m_instance->ProcessWindowMessages() && callable() ) {}
}
