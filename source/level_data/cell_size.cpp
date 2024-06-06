#include "pch.h"
#include "cell_size.h"

cell_size::cell_size(int width, int height) : m_width { width }, m_height { height }
{
}

auto cell_size::CellPosition(cell_id cellId) const noexcept -> POINT_2I
{
  auto position = cellId.Position();
  return { position.x * m_width, position.y * m_height };
}

auto cell_size::CellRect(cell_id cellId) const noexcept -> RECT_I
{
  auto position = CellPosition(cellId);
  return RECT_I { position.x - m_width / 2, position.y - m_height / 2, position.x + m_width / 2, position.y + m_height / 2 };
}
