#pragma once

#include "geometry.h"
#include "level_grid_cell_generator.h"

class asteroid_generator
{

public:

  asteroid_generator(int cellSize);
  [[nodiscard]] auto Create(int gridX, int gridY) -> game_closed_object;

private:

  std::uniform_int_distribution<int> m_asteroidAngleDist { 0, 359 };
  [[nodiscard]] auto GenerateAngle() -> float;

  float m_cellSize;
};
