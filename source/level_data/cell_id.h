#pragma once

#include "framework.h"

class cell_id
{

public:

  enum class relative_position { above, above_right, right, below_right, below, below_left, left, above_left };

  cell_id();
  cell_id(int column, int row);
  [[nodiscard]] auto Get(relative_position relativePosition) const noexcept -> cell_id;
  [[nodiscard]] auto Position(int cellWidth, int cellHeight) const noexcept -> POINT_2I;
  auto operator <(const cell_id& cellId) const -> bool;
  auto operator ==(const cell_id& cellId) const -> bool;

private:

  int m_column;
  int m_row;

};

inline cell_id::cell_id() : m_column { 0 }, m_row { 0 }
{
}

inline cell_id::cell_id(int column, int row) : m_column { column }, m_row { row }
{
}

inline auto cell_id::Get(relative_position relativePosition) const noexcept -> cell_id
{
  switch( relativePosition )
  {
    case relative_position::above:
      return { m_column, m_row - 1 };
    case relative_position::above_right:
      return { m_column + 1, m_row - 1 };
    case relative_position::right:
      return { m_column + 1, m_row };
    case relative_position::below_right:
      return { m_column + 1, m_row + 1 };
    case relative_position::below:
      return { m_column, m_row + 1 };
    case relative_position::below_left:
      return { m_column - 1, m_row + 1 };
    case relative_position::left:
      return { m_column - 1, m_row };
    case relative_position::above_left:
      return { m_column - 1, m_row - 1 };
    default:
      return { m_column, m_row };
  }
}

inline auto cell_id::Position(int cellWidth, int cellHeight) const noexcept -> POINT_2I
{
  return { m_column * cellWidth, m_row * cellHeight };
}

inline auto cell_id::operator <(const cell_id& cellId) const -> bool
{
  return m_row < cellId.m_row || ( m_row == cellId.m_row && m_column < cellId.m_column );
}

inline auto cell_id::operator==(const cell_id &cellId) const -> bool
{
  return m_row == cellId.m_row && m_column == cellId.m_column;
}
