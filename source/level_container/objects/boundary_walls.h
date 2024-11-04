#pragma once

#include "base_object.h"

class boundary_walls : public base_object
{

public:

  boundary_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept;

  [[nodiscard]] auto Level() const noexcept -> int;

private:

  int m_level;

};

inline boundary_walls::boundary_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept : base_object(position, scale, angle), m_level { level }
{
}

inline auto boundary_walls::Level() const noexcept -> int
{
  return m_level;
}
