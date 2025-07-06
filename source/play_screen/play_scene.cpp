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

auto play_scene::Render(const level_container& levelContainer, const play_state& playState) const -> void
{
  render_target::get()->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
  auto playerPosition = levelContainer.PlayerState().Position();
  RECT_F viewRect = { 0, playerPosition.y - 5000, 10000, playerPosition.y + 5000 };
  render_target::get()->SetTransform(RenderTransform(viewRect));
  RenderLevelContainer(levelContainer);
}

[[nodiscard]] auto play_scene::RenderTransform(const level_container& levelContainer) const noexcept -> D2D1::Matrix3x2F
{
  auto cameraPosition = CameraPosition(levelContainer);
  constexpr auto cameraAngle = 0;
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, cameraAngle, cameraPosition.scale, render_target::get()->GetSize() };
  return cameraTransform.Get();
}

[[nodiscard]] auto play_scene::RenderTransform(RECT_F viewRect) const noexcept -> D2D1::Matrix3x2F
{

  auto renderTargetHeight = render_target::get()->GetSize().height;

  float viewHeight = viewRect.bottom - viewRect.top;
  float viewScale = renderTargetHeight / viewHeight;

  float translateX = -viewRect.left * viewScale;
  float translateY = -viewRect.top * viewScale;

  return D2D1::Matrix3x2F::Scale(viewScale, viewScale) * D2D1::Matrix3x2F::Translation(translateX, translateY);
}

auto play_scene::CameraPosition(const level_container& levelContainer) const -> camera_sequence::camera_position
{
  auto&& playerState = levelContainer.PlayerState();
  auto playerPosition = playerState.Position();
  return camera_sequence::camera_position { 0.0f, playerPosition.y, m_cameraZoom };
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
  return renderTargetSize.height / 10000.0f;
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
