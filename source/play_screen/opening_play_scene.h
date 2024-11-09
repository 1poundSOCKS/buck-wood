#pragma once

#include "play_scene.h"

class opening_play_scene : public play_scene
{

public:

  opening_play_scene(auto&&...args);

  auto Begin(const level_container& levelContainer) -> void override;
  auto Update(const level_container& levelContainer, int64_t ticks) -> void override;
  auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool override;

private:

  camera_sequence m_cameraSequence;
  __int64 m_ticks { 0 };

};

opening_play_scene::opening_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
{
  m_renderLevelTitle = true;
}

inline auto opening_play_scene::Begin(const level_container& levelContainer) -> void
{
  auto&& playerState = levelContainer.PlayerState();
  auto playerPosition = playerState.Position();
  auto playCameraZoom = GetPlayCameraZoom();
  
  m_cameraSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 5.0f };
  m_cameraSequence.AddMove( { playerPosition.x, playerPosition.y, playCameraZoom }, performance_counter::CalculateTicks(3) );
  SetCameraZoom(m_cameraSequence.GetScale(0));

  m_renderTransform = RenderTransform(levelContainer);
}

inline auto opening_play_scene::Update(const level_container& levelContainer, int64_t ticks) -> void
{
  SetCameraZoom(m_cameraSequence.GetScale(m_ticks));
  play_scene::Update(levelContainer, 0);
  m_ticks += ticks;
}

inline auto opening_play_scene::Complete(const level_container& levelContainer, const play_state& playState) const -> bool
{
  return m_ticks < m_cameraSequence.GetTotalTicks() ? false : true;
}
