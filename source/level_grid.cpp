#include "pch.h"
#include "level_grid.h"

level_grid::level_grid(int columnWidth, int rowHeight, int left, int top, int right, int bottom) : m_columnWidth { columnWidth }, m_rowHeight { rowHeight }
{
  m_leftColumn = left / columnWidth - 1;
  m_rightColumn = right / columnWidth + 1;
  m_topRow = top / rowHeight - 1;
  m_bottomRow = bottom / rowHeight + 1;
}

auto level_grid::ColumnWidth() const -> int
{
  return m_columnWidth;
}

auto level_grid::RowHeight() const -> int
{
  return m_rowHeight;
}

auto level_grid::LeftColumn() const -> int
{
  return m_leftColumn;
}

auto level_grid::TopRow() const -> int
{
  return m_topRow;
}

auto level_grid::RightColumn() const -> int
{
  return m_rightColumn;
}

auto level_grid::BottomRow() const -> int
{
  return m_bottomRow;
}
