#pragma once

#include "framework.h"
#include "base_scene.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "play_menu_controller.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"
#include "level_radar.h"
#include "level_title.h"
#include "play_state.h"
#include "renderers.h"

class play_scene : public base_scene
{

public:

  play_scene(std::shared_ptr<play_state> playState);
  virtual ~play_scene();

  auto Begin() -> void override;
  auto End() -> void override;
  auto Pause() -> void override;
  auto Resume() -> void override;
  auto Update(__int64 ticks) -> bool override;
  auto Render() const -> void override;

protected:

  [[nodiscard]] auto RenderTransform() const noexcept -> D2D1::Matrix3x2F;
  auto RenderLevelContainer() const -> void;
  auto PlaySoundEffects() const -> void;
  auto SetCameraZoom(float value) -> void;
  auto GetRenderTargetView() const -> D2D1_RECT_F;
  auto CameraPosition() const -> camera_sequence::camera_position;
  [[nodiscard]] auto GetPlayCameraZoom() const -> float;
  static auto GetRenderTargetView(D2D1::Matrix3x2F transform) -> D2D1_RECT_F;

private:

  static constexpr float m_playerDestinationRange { 400 };
  static constexpr float m_targetRange { 600 };

protected:

  bool m_paused { false };
  std::shared_ptr<play_state> m_playState;
  level_title m_levelTitle;
  bool m_renderLevelTitle { false };
  float m_cameraZoom { 0.4f };
  D2D1::Matrix3x2F m_renderTransform { RenderTransform() };

};
