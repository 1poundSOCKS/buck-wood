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

  auto v = std::ranges::views::transform(GetDefaultTargetGeometryData(), [x, y](const auto& point)
  {
    return game_point { point.x + x, point.y + y };
  });

  m_geometry.Load(v);
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
