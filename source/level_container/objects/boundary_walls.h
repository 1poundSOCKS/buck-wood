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
