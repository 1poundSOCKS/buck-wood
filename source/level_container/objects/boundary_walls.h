#pragma once

#include "base_object.h"
#include "level_data.h"

class boundary_walls : public base_object
{

public:

  boundary_walls(POINT_2F position, SCALE_2F scale, float angle, int level) noexcept;

  [[nodiscard]] auto Level() const noexcept -> int;
  auto GetPointData(auto pointDataInserter) const noexcept -> bool;

private:

  int m_level;

};

auto boundary_walls::GetPointData(auto pointDataInserter) const noexcept -> bool
{
  return level_data::LoadBoundaryData(m_level, pointDataInserter);
}
