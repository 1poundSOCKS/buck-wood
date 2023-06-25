#include "pch.h"
#include "reload_timer.h"

reload_timer::reload_timer(float reloadTime) : m_reloadTime { reloadTime }
{
}

auto reload_timer::Update(float interval) -> bool
{
  auto reloaded = ( m_currentTime += interval ) > m_reloadTime;

  if( reloaded )
  {
    m_currentTime -= m_reloadTime;
  }

  return reloaded;
}
