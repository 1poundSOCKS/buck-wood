#pragma once

class level_grid
{

public:

  level_grid() = default;
  level_grid(int columnWidth, int rowHeight, int left, int top, int right, int bottom);
  auto operator==(const level_grid&) const -> bool = default;

  auto ColumnWidth() const -> int;
  auto RowHeight() const -> int;
  auto LeftColumn() const -> int;
  auto TopRow() const -> int;
  auto RightColumn() const -> int;
  auto BottomRow() const -> int;

private:

  int m_columnWidth { 0 };
  int m_rowHeight { 0 };
  int m_leftColumn { 0 };
  int m_topRow { 0 };
  int m_rightColumn { 0 };
  int m_bottomRow { 0 };

};
