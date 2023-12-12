#pragma once

#include "level_grid_cell.h"

class const_level_grid_iterator
{

  friend class level_grid;

public:

  enum class type { none, begin, end };

  const_level_grid_iterator() = default;
  using difference_type = std::ptrdiff_t;
  using value_type = level_grid_cell;

  auto operator++() -> const_level_grid_iterator&;
  auto operator++(int) -> const_level_grid_iterator;
  auto operator*() const -> const level_grid_cell&;
  auto operator==(const const_level_grid_iterator& i) const -> bool;

private:
  
  const_level_grid_iterator(const level_grid* levelGrid, type iteratorType);

  const level_grid* m_levelGrid { nullptr };
  type m_type { type::none };
  int m_currentRow { 0 };
  int m_currentColumn { 0 };
  level_grid_cell m_currentCell;

};

class level_grid
{

  friend class const_level_grid_iterator;

public:

  level_grid() = default;
  level_grid(int columnWidth, int leftColumn, int rightColumn, int rowHeight, int topRow, int bottomRow);
  level_grid(int columnWidth, int rowHeight, float left, float top, float right, float bottom);
  auto operator==(const level_grid&) const -> bool = default;

  auto ColumnWidth() const -> int;
  auto RowHeight() const -> int;
  auto LeftColumn() const -> int;
  auto TopRow() const -> int;
  auto RightColumn() const -> int;
  auto BottomRow() const -> int;
  auto GetCell(int column, int row) const -> level_grid_cell;

  [[nodiscard]] auto LeftBorder() const -> int;
  [[nodiscard]] auto TopBorder() const -> int;
  [[nodiscard]] auto RightBorder() const -> int;
  [[nodiscard]] auto BottomBorder() const -> int;
  [[nodiscard]] auto Centre() const -> game_point;

  auto begin() const -> const_level_grid_iterator;
  auto end() const -> const_level_grid_iterator;

private:

  auto Initialize(int left, int top, int right, int bottom) -> void;

  int m_columnWidth { 0 };
  int m_rowHeight { 0 };
  int m_leftColumn { 0 };
  int m_topRow { 0 };
  int m_rightColumn { 0 };
  int m_bottomRow { 0 };

};

[[nodiscard]] inline auto level_grid::LeftBorder() const -> int
{
  return m_leftColumn * m_columnWidth - m_columnWidth / 2;
}

[[nodiscard]] inline auto level_grid::TopBorder() const -> int
{
  return m_topRow * m_rowHeight - m_rowHeight / 2;
}

[[nodiscard]] inline auto level_grid::RightBorder() const -> int
{
  return m_rightColumn * m_columnWidth + m_columnWidth / 2;
}

[[nodiscard]] inline auto level_grid::BottomBorder() const -> int
{
  return m_bottomRow * m_rowHeight + m_rowHeight / 2;
}

[[nodiscard]] inline auto level_grid::Centre() const -> game_point
{
  auto x = ( static_cast<float>(LeftBorder()) + static_cast<float>(RightBorder()) ) / 2;
  auto y = ( static_cast<float>(TopBorder()) + static_cast<float>(BottomBorder()) ) / 2;
  return { x, y };
}
