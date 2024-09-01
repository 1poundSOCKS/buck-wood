#pragma once

#include "cell_id.h"
#include "cell_size.h"

class level_cell_bounds
{

public:

  level_cell_bounds();
  level_cell_bounds(std::ranges::input_range auto&& cellIds);
  
  [[nodiscard]] auto Left(cell_size cellSize) const noexcept -> int;
  [[nodiscard]] auto Top(cell_size cellSize) const noexcept -> int;
  [[nodiscard]] auto Right(cell_size cellSize) const noexcept -> int;
  [[nodiscard]] auto Bottom(cell_size cellSize) const noexcept -> int;

private:

  cell_id m_topLeft;
  cell_id m_bottomRight;
  
};
