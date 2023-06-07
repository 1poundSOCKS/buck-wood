#include "pch.h"
#include "mine.h"
#include "shape_generator.h"

mine::mine(float x, float y) : m_position { x, y }
{
  game_rect rect = { { x - 20, y - 20 }, { x + 20, y + 20} };
  m_geometry.Load( shape_generator<clean_shape> { rect, 4 } );
}

[[nodiscard]] auto mine::Position() const -> game_point
{
  return m_position;
}

[[nodiscard]] auto mine::Geometry() const -> const path_geometry&
{
  return m_geometry;
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
