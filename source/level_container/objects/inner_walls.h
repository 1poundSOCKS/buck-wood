#pragma once

#include "base_object.h"

class inner_walls : public base_object
{

public:

  inner_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept;

  [[nodiscard]] auto Level() const noexcept -> int;

private:

  int m_level;

};

inline inner_walls::inner_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept : base_object(position, scale, angle), m_level { level }
{
}

inline auto inner_walls::Level() const noexcept -> int
{
  return m_level;
}
