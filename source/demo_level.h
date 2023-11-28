#pragma once

#include "geometry.h"
#include "level_grid.h"

class demo_level
{

public:

  demo_level(const level_grid& levelGrid);

  auto begin() const -> std::vector<game_point>::const_iterator;
  auto end() const -> std::vector<game_point>::const_iterator;

private:

  std::vector<game_point> m_points;

};
