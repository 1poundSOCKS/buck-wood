#pragma once

#include "framework.h"
#include "level_types.h"
#include "valid_cell.h"
#include "linear_allocator.h"

class level_cell_collection
{

public:

  using key_type = std::tuple<int, int>;
  using collection_allocator_type = linear_allocator<std::pair<const key_type, valid_cell>, size_t { 200 }>;
  using collection_type = std::map<key_type, valid_cell, std::less<key_type>, collection_allocator_type>;
  enum class cell_type { empty, wall, floor };
  using cell_id = key_type;

public:

  level_cell_collection(int cellWidth, int cellHeight);

  [[nodiscard]] auto Get() const noexcept -> const collection_type&;
  [[nodiscard]] auto Get(cell_id cellId) const -> const valid_cell&;

  auto Add(int x, int y, level_cell_type cellType) noexcept -> void;
  auto AddWalls() noexcept -> void;

  [[nodiscard]] auto CellType(POINT_2F position) const -> cell_type;
  [[nodiscard]] auto CellType(collection_type::const_iterator cell) const -> cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;
  [[nodiscard]] auto CellRect(cell_id cellId) const -> RECT_F;

  [[nodiscard]] auto MinColumn() const noexcept -> int;
  [[nodiscard]] auto MaxColumn() const noexcept -> int;
  [[nodiscard]] auto MinRow() const noexcept -> int;
  [[nodiscard]] auto MaxRow() const noexcept -> int;

  [[nodiscard]] auto IsTypeOf(cell_id cellId, cell_type cellType) const noexcept -> bool;
  [[nodiscard]] auto CellPosition(int x, int y) const noexcept -> POINT_2F;

  [[nodiscard]] auto UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F;

  auto EnumerateCells(auto&& visitor) -> void;

private:

  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;
  [[nodiscard]] auto CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>;
  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;

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

inline auto level_cell_collection::Get(cell_id cellId) const -> const valid_cell &
{
  return m_cells.at(cellId);
}

auto level_cell_collection::EnumerateCells(auto &&visitor) -> void
{
  auto cells = std::ranges::views::transform(m_cells, [](const auto& cell) -> valid_cell
  {
    const auto& [key, value] = cell;
    return value;
  });

  for( const auto& cell : cells )
  {
    visitor(cell);
  }
}
