#pragma once

#include "play_scene.h"
#include "stopwatch.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
  {
  }

  auto Begin() -> void override
  {
    SetCameraZoom(GetPlayCameraZoom());
    m_playState->LevelContainer().SetPlayerActive(false);
    m_playState->LevelContainer().SetPlayerInvulnerable(true);
  }

  auto Update(int64_t ticks) -> bool override
  {
    play_scene::Update(ticks);
    return m_stopwatch.Update(ticks);
  }

private:

  stopwatch m_stopwatch { 3.0f };

};
