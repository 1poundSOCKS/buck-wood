#include "pch.h"
#include "level_grid.h"

auto const_level_grid_iterator::operator++() -> const_level_grid_iterator&
{
  if( ++m_currentColumn > m_levelGrid->m_rightColumn )
  {
    m_currentColumn = m_levelGrid->m_leftColumn;

    if( ++m_currentRow > m_levelGrid->m_bottomRow )
    {
      m_type = type::end;
    }
  }

  m_currentCell = m_levelGrid->GetCell(m_currentColumn, m_currentRow);

  return *this;
}

auto const_level_grid_iterator::operator++(int) -> const_level_grid_iterator
{
  auto tmp = *this;

  if( ++m_currentColumn > m_levelGrid->m_rightColumn )
  {
    m_currentColumn = m_levelGrid->m_leftColumn;

    if( ++m_currentRow > m_levelGrid->m_bottomRow )
    {
      m_type = type::end;
    }
  }

  m_currentCell = m_levelGrid->GetCell(m_currentColumn, m_currentRow);

  return tmp;
}

auto const_level_grid_iterator::operator*() const -> const level_grid_cell&
{
  return m_currentCell;
}

auto const_level_grid_iterator::operator==(const const_level_grid_iterator& i) const -> bool
{
  if( m_type == type::end && i.m_type == type::end )
  {
    return true;
  }
  else if( m_type == type::end || i.m_type == type::end )
  {
    return false;
  }
  else
  {
    return m_currentRow == i.m_currentRow && m_currentColumn == i.m_currentColumn;
  }
}

const_level_grid_iterator::const_level_grid_iterator(const level_grid* levelGrid, type iteratorType) : 
  m_type { iteratorType }, m_levelGrid { levelGrid }, m_currentRow { levelGrid->m_topRow }, m_currentColumn { levelGrid->m_leftColumn }, 
  m_currentCell { levelGrid->GetCell(m_currentColumn, m_currentRow) }
{
}

level_grid::level_grid(int columnWidth, int rowHeight, int left, int top, int right, int bottom) : m_columnWidth { columnWidth }, m_rowHeight { rowHeight }
{
  m_leftColumn = left / columnWidth - 2;
  m_rightColumn = right / columnWidth + 1;
  m_topRow = top / rowHeight - 2;
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

auto level_grid::GetCell(int column, int row) const -> level_grid_cell
{
  if( column < m_leftColumn || column > m_rightColumn || row < m_topRow || row > m_bottomRow )
  {
    return { 0, 0, 0, 0 };
  }
  else
  {
    auto cx = m_columnWidth / 2;
    auto cy = m_rowHeight / 2;
    auto centreX = column * m_columnWidth;
    auto centreY = row * m_rowHeight;

    return { centreX - cx + 1, centreY - cy + 1, centreX + cx, centreY + cy };
  }
}

auto level_grid::begin() const -> const_level_grid_iterator
{
  return { this, const_level_grid_iterator::type::begin };
}

auto level_grid::end() const -> const_level_grid_iterator
{
  return { this, const_level_grid_iterator::type::end };
}
