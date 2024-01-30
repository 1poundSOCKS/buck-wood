#pragma once

#include "framework.h"
#include "base_scene.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "play_menu_controller.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"
#include "level_radar.h"
#include "player_shields.h"
#include "play_events.h"

class play_scene : public base_scene
{

public:

  using level_container_ptr = std::shared_ptr<level_container>;

  play_scene(const level_container_ptr& levelContainer, play_events playEvents);
  virtual ~play_scene();

  auto Begin() -> void override;
  auto End() -> void override;
  auto Pause() -> void override;
  auto Resume() -> void override;
  auto Update(__int64 ticks) -> bool override;
  auto Render() const -> void override;

  auto RenderTransform() const -> D2D1::Matrix3x2F;
  auto LevelContainer() const -> level_container_ptr;

protected:

  auto PlaySoundEffects() const -> void;
  auto SetCameraZoom(float value) -> void;
  auto GetRenderTargetView() const -> D2D1_RECT_F;
  static auto GetRenderTargetView(D2D1::Matrix3x2F transform) -> D2D1_RECT_F;

private:

  static constexpr float m_playerDestinationRange { 400 };
  static constexpr float m_targetRange { 600 };

protected:

  level_container_ptr m_levelContainer;
  play_events m_playEvents;
  float m_cameraZoom { 1 };
  std::optional<D2D1_POINT_2F> m_playerDestination;
  // D2D1_POINT_2F m_targetOffset { 0 , 0 };
  std::optional<D2D1_POINT_2F> m_targetPosition;

};
