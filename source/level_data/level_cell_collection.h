#pragma once

#include "framework.h"
#include "level_types.h"
#include "cell_size.h"
#include "level_cell_item.h"

class level_cell_collection
{

public:

  level_cell_collection(cell_size cellSize);

  [[nodiscard]] auto Get(cell_id cellId) const -> level_cell_item;

  auto Add(cell_id cellId, level_cell_type cellType) noexcept -> void;

  [[nodiscard]] auto CellType(POINT_2F position) const -> level_cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;
  [[nodiscard]] auto CellRect(cell_id cellId) const -> RECT_F;

  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  [[nodiscard]] auto CellPosition(cell_id cellId) const noexcept -> POINT_2F;

  [[nodiscard]] auto UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F;

  auto Enumerate(auto&& visitor) -> void;

private:

  using key_type = cell_id;
  using map_entry_type = std::pair<const key_type, level_cell_type>;
  using cell_allocator_type = custom_allocator<map_entry_type>;
  using collection_allocator_type = custom_allocator<map_entry_type>;
  using collection_type = std::map<key_type, level_cell_type, std::less<key_type>, collection_allocator_type>;

private:

  [[nodiscard]] auto CellType(collection_type::const_iterator cell) const -> level_cell_type;
  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;
  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;
  static [[nodiscard]] auto ExpandRect(RECT_I rect, SIZE_F size) -> RECT_F;
  auto InsertCell(cell_id cellId, cell_id::relative_position position, POINT_2F cellPosition, auto&& inserter) noexcept -> void;

private:

  cell_size m_cellSize;

  custom_allocator_state m_cellBuffer;
  cell_allocator_type m_cellAllocator;
  collection_type m_cells;
  collection_type m_walls;

};

inline auto level_cell_collection::Get(cell_id cellId) const -> level_cell_item
{
  auto position = m_cellSize.CellPosition(cellId);

  auto cellEntry = m_cells.find(cellId);

  if( cellEntry == std::end(m_cells) )
  {
    return { cellId, level_cell_type::none, position };
  }
  else
  {
    const auto& [id,type] = *cellEntry;
    return { cellId, type, position };
  }
}

auto level_cell_collection::Enumerate(auto &&visitor) -> void
{
  auto cells = std::ranges::views::transform(m_cells, [this](auto& cell) -> level_cell_item
  {
    auto& [id, type] = cell;
    auto position = m_cellSize.CellPosition(id);
    return { id, type, position };
  });

  for( auto cell : cells )
  {
    visitor(cell);
  }

  m_walls.clear();

  for( const auto& cell : cells )
  {
    InsertCell(cell.CellId(), cell_id::relative_position::above, cell.Position(), std::inserter(m_walls, std::end(m_walls)));
    InsertCell(cell.CellId(), cell_id::relative_position::right, cell.Position(), std::inserter(m_walls, std::end(m_walls)));
    InsertCell(cell.CellId(), cell_id::relative_position::below, cell.Position(), std::inserter(m_walls, std::end(m_walls)));
    InsertCell(cell.CellId(), cell_id::relative_position::left, cell.Position(), std::inserter(m_walls, std::end(m_walls)));
  }

  auto walls = std::ranges::views::transform(m_walls, [this](auto& cell) -> level_cell_item
  {
    auto& [id, type] = cell;
    auto position = m_cellSize.CellPosition(id);
    return { id, type, position };
  });

  for( auto wall : walls )
  {
    visitor(wall);
  }
}

auto level_cell_collection::InsertCell(cell_id cellId, cell_id::relative_position position, POINT_2F cellPosition, auto &&inserter) noexcept -> void
{
  auto aboveCellId = cellId.Get(position);
  auto aboveCellItem = Get(aboveCellId);
  
  if( aboveCellItem.Type() == level_cell_type::none )
  {
    inserter = std::make_pair(aboveCellId, level_cell_type::wall);
  }
}
