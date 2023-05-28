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

level_target::level_target(float x, float y) : m_data { std::make_shared<data>() }
{
  m_data->position = { x, y };

  std::vector<game_point> points;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(x, y));

  game_closed_object object;
  object.Load(points.cbegin(), points.cend());
  m_geometry.Load(object);

  m_collisionData = collision_data { points.cbegin(), points.cend() };
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);

  Initialize(framework::renderTarget().get());
}

level_target::level_target(const game_closed_object& object)
{
  m_geometry.Load(object);

  m_collisionData = collision_data { object.points.cbegin(), object.points.cend() };
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);

  Initialize(framework::renderTarget().get());
}

[[nodiscard]] auto level_target::Position() const -> game_point
{
  return m_data->position;
}

auto level_target::SetActivated(event_activated eventActivated) -> void
{
  m_eventActivated = eventActivated;
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_data->activated;
}

auto level_target::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_brushNotActivated = screen_render_brush_green.CreateBrush(renderTarget);
  m_brushActivated =  screen_render_brush_red.CreateBrush(renderTarget);
  m_brushCentre =  screen_render_brush_dark_grey.CreateBrush(renderTarget);
}

auto level_target::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_target::Render(D2D1_RECT_F viewRect) const -> void
{
  framework::renderTarget()->FillGeometry(m_geometry.Get(), m_brushCentre.get());
  const auto& targetBrush = m_data->activated ? m_brushActivated : m_brushNotActivated;
  framework::renderTarget()->DrawGeometry(m_geometry.Get(), targetBrush.get(), 8.0f);
}

[[nodiscard]] auto level_target::GetCollisionData() const -> const collision_data&
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
  if( effect.GetProperty(collision_effect::activates_target) && !m_data->activated )
  {
    m_data->activated = true;
    m_eventActivated();
  }
}

[[nodiscard]] auto level_target::Destroyed() const -> bool
{
  return false;
}
