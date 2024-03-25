#pragma once

#include "reload_timer.h"
#include "level_container.h"

class level_update_event
{

public:

  level_update_event(float wait, std::function<void(level_container*)> event);
  auto Update(float interval, level_container* levelContainer) -> bool;

private:

  reload_timer m_timer;
  std::function<void(level_container*)> m_event;

};
