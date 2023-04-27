#include "pch.h"
#include "level_target.h"
#include "render_brush_defs.h"

static const float defaultTargetSize = 40;

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
  return GetTargetGeometryData(40);
}

level_target::level_target(float x, float y, std::function<void()> eventTargetActivated) : m_eventTargetActivated(eventTargetActivated)
{
  std::vector<game_point> points;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(x, y));

  CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(m_shape));

  m_collisionData = collision_data { points.cbegin(), points.cend() };
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
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

[[nodiscard]] auto level_target::LevelIsComplete() const -> bool
{
  return m_activated;
}

auto level_target::Render(D2D1_RECT_F viewRect) const -> void
{
  std::vector<render_line> renderLines;
  CreateRenderLines(m_shape.cbegin(), m_shape.cend(), std::back_inserter(renderLines), m_activated ? m_brushActivated.get() : m_brushNotActivated.get(), 6);
  RenderLines(m_renderTarget.get(), renderLines.cbegin(), renderLines.cend());
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
    m_eventTargetActivated();
  }
}

[[nodiscard]] auto level_target::Destroyed() const -> bool
{
  return false;
}
