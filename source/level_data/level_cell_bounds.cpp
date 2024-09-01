#include "pch.h"
#include "level_cell_bounds.h"

level_cell_bounds::level_cell_bounds()
{
}

auto level_cell_bounds::Left(cell_size cellSize) const noexcept -> int
{
  return cellSize.CellPosition(m_topLeft).x;
}

auto level_cell_bounds::Top(cell_size cellSize) const noexcept -> int
{
  return cellSize.CellPosition(m_topLeft).y;
}

auto level_cell_bounds::Right(cell_size cellSize) const noexcept -> int
{
  return cellSize.CellPosition(m_bottomRight).x;
}

auto level_cell_bounds::Bottom(cell_size cellSize) const noexcept -> int
{
  return cellSize.CellPosition(m_bottomRight).y;
}
