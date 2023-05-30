#include "pch.h"
#include "level_target.h"
#include "render_brush_defs.h"
#include "renderers.h"

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
  m_position = { x, y };

  std::vector<game_point> points;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(x, y));

  game_closed_object object;
  object.Load(points.cbegin(), points.cend());
  m_geometry.Load(object);
}

level_target::level_target(const game_closed_object& object)
{
  m_geometry.Load(object);
}

[[nodiscard]] auto level_target::Position() const -> game_point
{
  return m_position;
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_activated;
}

[[nodiscard]] auto level_target::Geometry() const -> const path_geometry&
{
  return m_geometry;
}

auto level_target::Activate() -> void
{
  m_activated = true;
}
