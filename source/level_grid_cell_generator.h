#pragma once

class level_grid_cell_generator
{

public:

  struct grid_position
  {
    int x = 0;
    int y = 0;

    auto operator<=>(const grid_position& gridPosition) const = default;
  };

  level_grid_cell_generator();
  auto Create() -> grid_position;

private:

  std::uniform_int_distribution<int> m_cellCoodinateValue { -4, 5 };

  [[nodiscard]] auto GenerateCoordinate() -> int;

  std::set<grid_position> m_cells;
};
