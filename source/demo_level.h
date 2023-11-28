#pragma once

#include "geometry.h"
#include "level_grid.h"

class demo_level
{

public:

  demo_level(const level_grid& levelGrid);

  [[nodiscard]] auto Boundary() const -> const std::vector<game_point>&;

private:

  std::vector<game_point> m_boundary;

};
