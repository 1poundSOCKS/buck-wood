#pragma once

#include "play_scene.h"
#include "stopwatch.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
  }

  auto Begin() -> void override
  {
    SetCameraZoom(0.5);
  }

  auto Update(int64_t ticks) -> bool override
  {
    play_scene::Update(ticks);
    return m_stopwatch.Update(ticks);
  }

private:

  stopwatch m_stopwatch { 3.0f };

};
