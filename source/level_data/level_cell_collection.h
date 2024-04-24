#pragma once

#include "framework.h"
#include "valid_cell.h"

class level_cell_collection
{

public:

  using key_type = std::tuple<int, int>;
  using collection_type = std::map<key_type, valid_cell>;
  enum class cell_type { wall, floor };

public:

  level_cell_collection(int cellWidth, int cellHeight);
  [[nodiscard]] auto Get() const noexcept -> collection_type;

  auto Add(int x, int y) noexcept -> void;
  [[nodiscard]] auto CellType(POINT_2F position) const -> cell_type;

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

inline [[nodiscard]] auto level_cell_collection::Get() const noexcept -> collection_type
{
  return m_cells;
}
