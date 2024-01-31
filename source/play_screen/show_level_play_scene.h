#pragma once

#include "play_scene.h"

class show_level_play_scene : public play_scene
{

public:

  show_level_play_scene(const level_container_ptr& levelContainer, play_events playEvents) : play_scene(levelContainer, playEvents)
  {
  }

  auto Update(int64_t ticks) -> bool override
  {
    SetCameraZoom(0.1f);
    return gamepad_reader::button_pressed(XINPUT_GAMEPAD_A) ? false : true;
  }

};
