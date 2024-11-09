#include "pch.h"
#include "play_scene.h"
#include "hud_target.h"
#include "audio_events.h"
#include "line_to_target.h"
#include "game_settings.h"
#include "game_state.h"
#include "energy_bar_rect.h"

play_scene::play_scene(const level_container& levelContainer) : 
  m_levelTitle { game_state::level_index() }, m_cameraZoom { GetPlayCameraZoom() }, m_renderTransform { RenderTransform(levelContainer) }
{
}

play_scene::~play_scene()
{
}

auto play_scene::Begin(const level_container& levelContainer) -> void
{
}

auto play_scene::End(const level_container& levelContainer) -> void
{
}

auto play_scene::Pause(const level_container& levelContainer) -> void
{
  m_paused = true;
}

auto play_scene::Resume(const level_container& levelContainer) -> void
{
  m_paused = false;
}

auto play_scene::Render(const level_container& levelContainer) const -> void
{
  render_target::get()->Clear(D2D1::ColorF(0.15f, 0.15f, 0.15f, 1.0f));
  render_target::get()->SetTransform(RenderTransform(levelContainer));
  RenderLevelContainer(levelContainer);
}

[[nodiscard]] auto play_scene::RenderTransform(const level_container& levelContainer) const noexcept -> D2D1::Matrix3x2F
{
  auto cameraPosition = CameraPosition(levelContainer);
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  return cameraTransform.Get();
}

auto play_scene::CameraPosition(const level_container& levelContainer) const -> camera_sequence::camera_position
{
  auto&& playerState = levelContainer.PlayerState();
  auto playerPosition = playerState.Position();
  return camera_sequence::camera_position { playerPosition.x, playerPosition.y, m_cameraZoom };
}

auto play_scene::RenderLevelContainer(const level_container& levelContainer) const -> void
{
  auto renderStart = performance_counter::QueryValue();
  renderer::render(levelContainer);
  auto renderEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
}

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}

[[nodiscard]] auto play_scene::GetPlayCameraZoom() const -> float
{
  auto renderTargetSize = render_target::get()->GetSize();
  return renderTargetSize.height / 6000.0f;
}

auto play_scene::GetRenderTargetView(const level_container& levelContainer) const -> D2D1_RECT_F
{
  return GetRenderTargetView(m_renderTransform);
}

auto play_scene::Complete(const level_container &levelContainer, const play_state& playState) const -> bool
{
  return true;
}

auto play_scene::GetRenderTargetView(D2D1::Matrix3x2F transform) -> D2D1_RECT_F
{
  auto renderTargetSize = render_target::get()->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };

  transform.Invert();
  auto topLeft = transform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = transform.TransformPoint(renderTargetBottomRight);
  auto bottomLeft = transform.TransformPoint({renderTargetTopLeft.x, renderTargetTopLeft.y});
  auto topRight = transform.TransformPoint({renderTargetTopLeft.x, renderTargetTopLeft.y});

  auto [minX, maxX] = std::minmax({topLeft.x, bottomRight.x, bottomLeft.x, topRight.x});
  auto [minY, maxY] = std::minmax({topLeft.y, bottomRight.y, bottomLeft.y, topRight.y});

  return { minX, minY, maxX, maxY };
}
