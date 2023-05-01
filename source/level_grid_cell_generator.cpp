#include "pch.h"
#include "level_grid_cell_generator.h"
#include "framework.h"

level_grid_cell_generator::level_grid_cell_generator()
{
  m_cells.emplace(grid_position { 0, 0 });
}

auto level_grid_cell_generator::Create() -> grid_position
{
  auto gridX = 0;
  auto gridY = 0;

  do
  {
    gridX = GenerateCoordinate();
    gridY = GenerateCoordinate();
  }
  while( m_cells.contains(grid_position { gridX, gridY } ));

  m_cells.emplace(grid_position { gridX, gridY });

  return { gridX, gridY };
}

[[nodiscard]] auto level_grid_cell_generator::GenerateCoordinate() -> int
{
  return m_cellCoodinateValue(framework::rng());
}
