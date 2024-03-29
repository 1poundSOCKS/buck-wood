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

// level_grid::level_grid(int columnWidth, int rowHeight, int left, int top, int right, int bottom) : m_columnWidth { columnWidth }, m_rowHeight { rowHeight }
// {
//   Initialize(left, top, right, bottom);
// }

level_grid::level_grid(int columnWidth, int rowHeight, float left, float top, float right, float bottom) : 
  m_columnWidth { columnWidth }, m_rowHeight { rowHeight }
{
  Initialize(static_cast<int>(left), static_cast<int>(top), static_cast<int>(right), static_cast<int>(bottom));
}

level_grid::level_grid(int columnWidth, int leftColumn, int rightColumn, int rowHeight, int topRow, int bottomRow) :
  m_columnWidth{ columnWidth }, m_leftColumn { leftColumn }, m_rightColumn { rightColumn },
  m_rowHeight { rowHeight }, m_topRow { topRow }, m_bottomRow { bottomRow }
{
}

auto level_grid::Initialize(int left, int top, int right, int bottom) -> void
{
  m_leftColumn = left / m_columnWidth;
  m_topRow = top / m_rowHeight;
  m_rightColumn = right / m_columnWidth;
  m_bottomRow = bottom / m_rowHeight;
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
    return { static_cast<float>(column * m_columnWidth), static_cast<float>(row * m_rowHeight), static_cast<float>(m_columnWidth), static_cast<float>(m_rowHeight) };
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
