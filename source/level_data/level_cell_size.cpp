#include "pch.h"
#include "level_cell_size.h"

level_cell_size::level_cell_size(int width, int height) : m_width { width }, m_height { height }
{
}

auto level_cell_size::CellPosition(cell_id cellId) const noexcept -> POINT_2I
{
  auto position = cellId.Position();
  return { position.x, position.y };
}
