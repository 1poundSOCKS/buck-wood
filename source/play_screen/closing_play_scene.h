#pragma once

#include "play_scene.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
  }

  auto Refresh(__int64 ticks) -> bool override
  {
    SetCameraZoom(0.5);
    play_scene::Refresh(ticks);
    return ( m_ticks -= ticks ) > 0 ? true : false;
  }

private:

  __int64 m_ticks { performance_counter::CalculateTicks(3) };

};
