#pragma once

#include "play_scene.h"

class opening_play_scene : public play_scene
{

public:

  opening_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
  }

  auto Begin() -> void override
  {
    auto playerPosition = m_levelContainer->PlayerPosition();
    m_cameraSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 0.1f };
    m_cameraSequence.AddMove( { playerPosition.x, playerPosition.y, 1 }, performance_counter::CalculateTicks(2) );
    SetCameraZoom(m_cameraSequence.GetScale(0));
  }

  auto Update(int64_t ticks) -> bool override
  {
    SetCameraZoom(m_cameraSequence.GetScale(m_ticks));
    play_scene::Update(0);
    return ( m_ticks += ticks ) < m_cameraSequence.GetTotalTicks() ? true : false;
  }

private:

  camera_sequence m_cameraSequence;
  __int64 m_ticks { 0 };

};
