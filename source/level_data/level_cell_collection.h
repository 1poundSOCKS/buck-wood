#pragma once

#include "framework.h"
#include "valid_cell.h"

class level_cell_collection
{

public:

  using key_type = std::tuple<int, int>;
  using collection_type = std::map<key_type, valid_cell>;
  enum class cell_type { empty, wall, floor };
  using cell_id = std::tuple<int, int>;

public:

  level_cell_collection(int cellWidth, int cellHeight);

  [[nodiscard]] auto Get() const noexcept -> const collection_type&;
  [[nodiscard]] auto Get(int column, int row) -> const valid_cell&;

  auto Add(valid_cell::cell_type cellType, int x, int y) noexcept -> void;
  auto AddWalls() noexcept -> void;

  [[nodiscard]] auto CellType(POINT_2F position) const -> cell_type;
  [[nodiscard]] auto CellType(collection_type::const_iterator cell) const -> cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;

  [[nodiscard]] auto MinColumn() const noexcept -> int;
  [[nodiscard]] auto MaxColumn() const noexcept -> int;
  [[nodiscard]] auto MinRow() const noexcept -> int;
  [[nodiscard]] auto MaxRow() const noexcept -> int;

private:

  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;
  [[nodiscard]] auto CellPosition(int x, int y) const noexcept -> POINT_2F;
  [[nodiscard]] auto CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>;

private:

  int m_cellWidth;
  int m_cellHeight;
  winrt::com_ptr<ID2D1PathGeometry> m_cellGeometry;
  collection_type m_cells;

};

inline [[nodiscard]] auto level_cell_collection::Get() const noexcept -> const collection_type&
{
  return m_cells;
}

// inline [[nodiscard]] auto level_cell_collection::Get(int column, int row) -> const valid_cell&
// {
//   m_Cells.find({columnm, row});
// }
