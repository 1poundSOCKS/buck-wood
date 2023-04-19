#include "pch.h"
#include "pausable_timer.h"
#include "clock_frequency.h"

pausable_timer::pausable_timer()
{

}

auto pausable_timer::Start(int64_t currentClockTicks) -> void
{
  m_startTicks = m_previousTicks = m_currentTicks = currentClockTicks;
  m_pausedTicks = 0;
}

auto pausable_timer::Pause() -> void
{
  m_paused = true;
}

auto pausable_timer::Stop() -> void
{
  m_stopped = true;
}

auto pausable_timer::Unpause() -> void
{
  m_paused = false;
}

auto pausable_timer::Update(int64_t currentTicks) -> int64_t
{
  if( !m_stopped )
  {
    m_previousTicks = m_currentTicks;
    m_currentTicks = currentTicks;
    auto elapsedTicks = m_currentTicks - m_previousTicks;
    m_pausedTicks += m_paused ? elapsedTicks : 0;
  }
  
  return m_currentTicks - m_startTicks - m_pausedTicks;
}
