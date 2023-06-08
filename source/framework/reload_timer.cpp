#include "pch.h"
#include "reload_timer.h"

reload_timer::reload_timer(int64_t reloadTicks) : m_reloadTicks { reloadTicks }
{
}

auto reload_timer::Update(int64_t ticks) -> bool
{
  auto reloaded = ( m_currentTicks += ticks ) > m_reloadTicks;

  if( reloaded )
  {
    m_currentTicks -= m_reloadTicks;
  }

  return reloaded;
}
