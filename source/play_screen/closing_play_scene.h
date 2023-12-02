#pragma once

#include "play_scene.h"
#include "stopwatch.h"

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
    return m_stopwatch.Update(ticks);
  }

private:

  stopwatch m_stopwatch { 3.0f };

};
