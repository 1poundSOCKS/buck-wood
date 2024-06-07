#pragma once

#include "framework.h"
#include "level_types.h"
#include "cell_size.h"
#include "level_cell_item.h"

class level_cell_collection
{

public:

  level_cell_collection(int cellWidth, int cellHeight);

  [[nodiscard]] auto Get(cell_id cellId) const -> level_cell_item;

  auto Add(cell_id cellId, level_cell_type cellType) noexcept -> void;
  auto AddWalls() noexcept -> void;

  [[nodiscard]] auto CellWidth() const -> int;
  [[nodiscard]] auto CellHeight() const -> int;
  [[nodiscard]] auto CellType(POINT_2F position) const -> level_cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;
  [[nodiscard]] auto CellRect(cell_id cellId) const -> RECT_F;

  [[nodiscard]] auto MinColumn() const noexcept -> int;
  [[nodiscard]] auto MaxColumn() const noexcept -> int;
  [[nodiscard]] auto MinRow() const noexcept -> int;
  [[nodiscard]] auto MaxRow() const noexcept -> int;

  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  [[nodiscard]] auto CellPosition(int x, int y) const noexcept -> POINT_2F;

  [[nodiscard]] auto UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F;

  auto EnumerateCells(auto&& visitor) const -> void;
  auto EnumerateWalls(auto&& visitor) const -> void;

private:

  using key_type = std::tuple<int32_t, int32_t>;
  using map_entry_type = std::pair<const key_type, level_cell_type>;
  using cell_allocator_type = custom_allocator<map_entry_type>;
  using collection_allocator_type = custom_allocator<map_entry_type>;
  using collection_type = std::map<key_type, level_cell_type, std::less<key_type>, collection_allocator_type>;

private:

  [[nodiscard]] auto Key(cell_id cellId) const noexcept -> key_type;
  [[nodiscard]] auto CellType(collection_type::const_iterator cell) const -> level_cell_type;
  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;
  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;
  static [[nodiscard]] auto ExpandRect(RECT_I rect, SIZE_F size) -> RECT_F;

private:

  int m_cellWidth;
  int m_cellHeight;
  cell_size m_cellSize;

  custom_allocator_state m_cellBuffer;
  cell_allocator_type m_cellAllocator;
  collection_type m_cells;

};

inline auto level_cell_collection::Get(cell_id cellId) const -> level_cell_item
{
  auto key = Key(cellId);
  auto [column, row] = key;
  auto position = m_cellSize.CellPosition(cellId);

  auto cellEntry = m_cells.find(key);

  if( cellEntry == std::end(m_cells) )
  {
    return { column, row, level_cell_type::none, position };
  }
  else
  {
    const auto& [id,type] = *cellEntry;
    return { column, row, type, position };
  }
}

auto level_cell_collection::EnumerateCells(auto &&visitor) const -> void
{
  auto cells = std::ranges::views::transform(m_cells, [this](const auto& cell) -> level_cell_item
  {
    const auto& [key, type] = cell;
    auto [column, row] = key;
    auto id = cell_id { column, row };
    auto position = m_cellSize.CellPosition(id);
    return { column, row, type, position };
  });

  for( const auto& cell : cells )
  {
    visitor(cell);
  }
}

inline auto level_cell_collection::EnumerateWalls(auto &&visitor) const -> void
{
  EnumerateCells([&visitor](const level_cell_item& cell)
  {
    if( cell.Type() == level_cell_type::wall )
    {
      visitor(cell);
    }
  });
}
