#include "pch.h"
#include "level_update_event.h"

level_update_event::level_update_event(float wait, std::function<void(level_container*)> event) : m_timer { wait }, m_event { event }
{
}

auto level_update_event::Update(float interval, level_container* levelContainer) -> bool
{
  bool complete = m_timer.Update(interval);

  if( complete )
  {
    m_event(levelContainer);
  }

  return complete;
}
