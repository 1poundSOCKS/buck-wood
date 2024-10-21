#pragma once

#include "play_scene.h"

class opening_play_scene : public play_scene
{

public:

  opening_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
  {
    m_renderLevelTitle = true;
  }

  auto Begin() -> void override
  {
    auto playerState = m_playState->LevelContainer().PlayerState();
    auto playerPosition = playerState ? playerState->Position() : POINT_2F { 0.0f, 0.0f };
    auto playCameraZoom = GetPlayCameraZoom();
    
    m_cameraSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 5.0f };
    m_cameraSequence.AddMove( { playerPosition.x, playerPosition.y, playCameraZoom }, performance_counter::CalculateTicks(3) );
    SetCameraZoom(m_cameraSequence.GetScale(0));

    m_renderTransform = RenderTransform();
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
