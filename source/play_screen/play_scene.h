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

  play_scene(const level_container& levelContainer);
  virtual ~play_scene();

  auto Begin(const level_container& levelContainer) -> void override;
  auto End(const level_container& levelContainer) -> void override;
  auto Pause(const level_container& levelContainer) -> void override;
  auto Resume(const level_container& levelContainer) -> void override;
  auto Render(const level_container& levelContainer, const play_state& playState) const -> void override;
  auto GetRenderTargetView(const level_container& levelContainer) const -> D2D1_RECT_F override;
  auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool override;

protected:

  [[nodiscard]] auto RenderTransform(const level_container& levelContainer) const noexcept -> D2D1::Matrix3x2F;
  [[nodiscard]] auto RenderTransform(RECT_F viewRect) const noexcept -> D2D1::Matrix3x2F;
  auto RenderLevelContainer(const level_container& levelContainer) const -> void;
  auto SetCameraZoom(float value) -> void;
  auto CameraPosition(const level_container& levelContainer) const -> camera_sequence::camera_position;
  [[nodiscard]] auto GetPlayCameraZoom() const -> float;
  static auto GetRenderTargetView(D2D1::Matrix3x2F transform) -> D2D1_RECT_F;

private:

  static constexpr float m_playerDestinationRange { 400 };
  static constexpr float m_targetRange { 600 };

protected:

  bool m_paused { false };
  std::shared_ptr<level_container> m_levelContainer;
  level_title m_levelTitle;
  bool m_renderLevelTitle { false };
  float m_cameraZoom { 5.0f };
  D2D1::Matrix3x2F m_renderTransform;

};
