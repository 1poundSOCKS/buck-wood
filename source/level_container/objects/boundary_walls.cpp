#include "pch.h"
#include "boundary_walls.h"

boundary_walls::boundary_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept : base_object(position, scale, angle), m_level { level }
{
}

auto boundary_walls::Level() const noexcept -> int
{
  return m_level;
}
