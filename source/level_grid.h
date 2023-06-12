#pragma once

#include "geometry.h"

struct level_grid_cell
{
public:

  level_grid_cell() = default;
  level_grid_cell(const level_grid_cell&) = default;
  level_grid_cell(float x, float y, float width, float height);

  auto Position() const -> const game_point&;
  auto Left() const -> float;
  auto Top() const -> float;
  auto Right() const -> float;
  auto Bottom() const -> float;

  auto Resize(float xRatio, float yRatio) -> void;

private:

  game_point m_position { 0, 0 };
  float m_width { 0 };
  float m_height { 0 };

};

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
  level_grid(int columnWidth, int rowHeight, int left, int top, int right, int bottom);
  level_grid(int columnWidth, int rowHeight, float left, float top, float right, float bottom);
  auto operator==(const level_grid&) const -> bool = default;

  auto ColumnWidth() const -> int;
  auto RowHeight() const -> int;
  auto LeftColumn() const -> int;
  auto TopRow() const -> int;
  auto RightColumn() const -> int;
  auto BottomRow() const -> int;
  auto GetCell(int column, int row) const -> level_grid_cell;

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
