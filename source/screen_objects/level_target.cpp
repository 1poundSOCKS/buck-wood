#include "pch.h"
#include "level_target.h"
#include "game_objects.h"
#include "render_defs.h"

target_state::target_state(float x, float y)
{
  std::vector<game_point> points;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(x, y));

  CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(shape));

  m_collisionData = collision_data { points.cbegin(), points.cend() };
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
}

auto target_state::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  brushNotActivated = screen_render_brush_green.CreateBrush(renderTarget);
  brushActivated =  screen_render_brush_red.CreateBrush(renderTarget);
}

auto target_state::Update(int64_t tickCount, play_event_inserter playEventInserter) -> void
{
}

[[nodiscard]] auto target_state::LevelIsComplete() const -> bool
{
  return activated;
}

auto target_state::Render(D2D1_RECT_F viewRect) const -> void
{
  std::vector<render_line> renderLines;
  CreateRenderLines(shape.cbegin(), shape.cend(), std::back_inserter(renderLines), activated ? brushActivated.get() : brushNotActivated.get(), 6);
  RenderLines(m_renderTarget.get(), renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto target_state::GetCollisionData() -> collision_data
{
  return m_collisionData;
}

[[nodiscard]] auto target_state::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return m_collisionData.HasCollidedWith(collisionData);
}

[[nodiscard]] auto target_state::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto target_state::ApplyCollisionEffect(const collision_effect& effect, play_event_inserter playEventInserter) -> void
{
  if( effect.GetProperty(collision_effect::activates_target) )
    activated = true;
}

[[nodiscard]] auto target_state::Destroyed() const -> bool
{
  return false;
}
