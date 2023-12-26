#pragma once

#include "framework.h"

template <typename screen_state_type>
class screen_container
{

public:

  screen_container(std::optional<int> fps, std::optional<int> toggleFullscreenKey) : 
    m_fps { fps }, m_toggleFullscreenKey { toggleFullscreenKey }, m_timerFrequency { performance_counter::QueryFrequency() }, 
    m_currentTime { performance_counter::QueryValue() }, m_previousTime { m_currentTime }
  {
    log::write(log::type::info, "create screen container");
  }

  auto operator()() -> bool
  {
    keyboard_reader::update();
    gamepad_reader::update();

#ifdef USE_FPS_FRAMETIME
    auto frameTime = m_fps ? m_timerFrequency / *m_fps : m_currentTime - m_previousTime;
#else
    auto frameTime = m_currentTime - m_previousTime;
#endif

    if( m_toggleFullscreenKey && keyboard_reader::pressed(*m_toggleFullscreenKey) )
    {
      BOOL fullScreen = FALSE;
      swap_chain::get()->GetFullscreenState(&fullScreen, nullptr);
      swap_chain::get()->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
    }

    auto keepOpen = screenState.Refresh(frameTime);

    m_previousTime = m_currentTime;
    m_currentTime = performance_counter::QueryValue();

    swap_chain::get()->Present(m_fps ? 1 : 0, 0);

    return keepOpen;
  }

private:

  std::optional<int> m_fps;
  std::optional<int> m_toggleFullscreenKey;
  __int64 m_timerFrequency { 0 };
  __int64 m_currentTime { 0 };
  __int64 m_previousTime { 0 };
  screen_state_type screenState;

};
