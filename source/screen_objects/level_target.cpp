#include "pch.h"
#include "level_target.h"
#include "render_brush_defs.h"

constexpr std::array<game_point, 4> GetTargetGeometryData(float size)
{
  float halfSize = size / 2;
  
  return {
    game_point { 0, -halfSize },
    game_point { halfSize, 0 },
    game_point { 0, halfSize },
    game_point { -halfSize, 0 }
  };
}

consteval std::array<game_point, 4> GetDefaultTargetGeometryData()
{
  return GetTargetGeometryData(30);
}

level_target::level_target(float x, float y)
{
  std::vector<game_point> points;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(x, y));

  game_closed_object object;
  object.Load(points.cbegin(), points.cend());
  m_geometry.Load(object);

  m_collisionData = collision_data { points.cbegin(), points.cend() };
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
}

auto level_target::SetActivated(event_activated eventActivated) -> void
{
  m_eventActivated = eventActivated;
}

auto level_target::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brushNotActivated = screen_render_brush_green.CreateBrush(renderTarget);
  m_brushActivated =  screen_render_brush_red.CreateBrush(renderTarget);
}

auto level_target::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_target::Render(D2D1_RECT_F viewRect) const -> void
{
  m_renderTarget->DrawGeometry(m_geometry.Get(), m_activated ? m_brushActivated.get() : m_brushNotActivated.get(), 5.0f);
}

[[nodiscard]] auto level_target::GetCollisionData() -> collision_data
{
  return m_collisionData;
}

[[nodiscard]] auto level_target::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return m_collisionData.HasCollidedWith(collisionData);
}

[[nodiscard]] auto level_target::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto level_target::ApplyCollisionEffect(const collision_effect& effect) -> void
{
  if( effect.GetProperty(collision_effect::activates_target) && !m_activated )
  {
    m_activated = true;
    m_eventActivated();
  }
}

[[nodiscard]] auto level_target::Destroyed() const -> bool
{
  return false;
}
