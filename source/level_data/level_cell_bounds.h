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
  cell_id m_leftmostCell;
  cell_id m_topCell;
  cell_id m_rightmostCell;
  cell_id m_bottomCell;
  
};

level_cell_bounds::level_cell_bounds(std::ranges::input_range auto &&cellIds)
{
  for( const auto& cellId : cellIds )
  {
    m_leftmostCell = cellId.IsLeftOf(m_leftmostCell) ? cellId : m_leftmostCell;
    m_topCell = cellId.IsAbove(m_topCell) ? cellId : m_topCell;
    m_rightmostCell = cellId.IsRightOf(m_rightmostCell) ? cellId : m_rightmostCell;
    m_bottomCell = cellId.IsBelow(m_bottomCell) ? cellId : m_bottomCell;
  }
}
