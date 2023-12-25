#pragma once

#include "framework.h"
#include "base_scene.h"
#include "screen_transform.h"
#include "sound_data.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "play_menu_controller.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"
#include "level_radar.h"
#include "player_shields.h"

class play_scene : public base_scene
{

public:

  using level_container_ptr = std::shared_ptr<level_container>;

  play_scene(const level_container_ptr& levelContainer);
  virtual ~play_scene();

  auto Begin() -> void override;
  auto End() -> void override;
  auto Pause() -> void override;
  auto Resume() -> void override;
  auto Update(__int64 ticks) -> bool override;
  auto Render() const -> void override;

  auto ScreenTransform() const -> screen_transform;
  auto LevelContainer() const -> level_container_ptr;

protected:

  [[nodiscard]] auto GetLevelInput() const -> level_input;
  auto PlaySoundEffects() const -> void;
  auto SetCameraZoom(float value) -> void;

protected:

  level_container_ptr m_levelContainer;
  level_container::update_events m_levelUpdateEvents;
  float m_cameraZoom { 1 };

};
