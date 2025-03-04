#pragma once

#include "framework.h"
#include "game_settings.h"

template <typename screen_state_type>
class screen_container
{

public:

  screen_container(bool enableVSync, std::optional<int> toggleFullscreenKey) : 
    m_enableVsync { enableVSync }, m_toggleFullscreenKey { toggleFullscreenKey }, m_timerFrequency { performance_counter::QueryFrequency() }, 
    m_currentTime { performance_counter::QueryValue() }, m_previousTime { m_currentTime }, m_vsyncFrameTime { performance_counter::QueryFrequency() / game_settings::swapChainRefreshRate() }
  {
    log::write(log::type::info, "create screen container");
  }

  auto operator()() -> bool
  {
    keyboard_reader::update();
    gamepad_reader::update();

    auto frameTime = m_enableVsync ? m_vsyncFrameTime : m_currentTime - m_previousTime;

    if( m_toggleFullscreenKey && keyboard_reader::pressed(*m_toggleFullscreenKey) )
    {
      BOOL fullScreen = FALSE;
      swap_chain::get()->GetFullscreenState(&fullScreen, nullptr);
      swap_chain::get()->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
    }

    auto keepOpen = m_screenState.Refresh(frameTime);

    m_previousTime = m_currentTime;
    m_currentTime = performance_counter::QueryValue();

    swap_chain::get()->Present(m_enableVsync ? 1 : 0, 0);

    return keepOpen;
  }

private:

  bool m_enableVsync;
  __int64 m_vsyncFrameTime;
  std::optional<int> m_toggleFullscreenKey;
  __int64 m_timerFrequency;
  __int64 m_currentTime;
  __int64 m_previousTime;
  screen_state_type m_screenState;

};
