#include "pch.h"
#include "reload_timer.h"

reload_timer::reload_timer(float reloadTime) : m_reloadTime { reloadTime }, m_currentTime { 0.0f }
{
}

auto reload_timer::Update(float interval) -> bool
{
  m_currentTime += interval;
  
  int reloadCount = 0;

  while( m_currentTime > m_reloadTime )
  {
    ++reloadCount;
    m_currentTime -= m_reloadTime;
  }

  return reloadCount > 0;
}

auto reload_timer::Reset() -> void
{
  m_currentTime = 0;
}
