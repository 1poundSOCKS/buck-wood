#pragma once

#include "cell_id.h"
#include "level_types.h"

class cell_collection
{

public:

  cell_collection(RECT_I bounds) noexcept;

  auto Set(cell_id cellId, level_cell_type cellType) noexcept -> void;
  auto Reset() noexcept -> void;

  [[nodiscard]] auto GetType(cell_id cellId) const noexcept -> level_cell_type;
  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  auto Enumerate(auto&& visitor) const noexcept -> void;

  [[nodiscard]] auto Bounds() const noexcept -> RECT_I;

  [[nodiscard]] auto FirstColumn() const noexcept -> int;
  [[nodiscard]] auto EndOfColumns() const noexcept -> int;

  [[nodiscard]] auto FirstRow() const noexcept -> int;
  [[nodiscard]] auto EndOfRows() const noexcept -> int;
  
  [[nodiscard]] auto ColumnTop(int column) const noexcept -> cell_id;
  [[nodiscard]] auto ColumnBottom(int column) const noexcept -> cell_id;

  [[nodiscard]] auto RowLeft(int row) const noexcept -> cell_id;
  [[nodiscard]] auto RowRight(int row) const noexcept -> cell_id;

  [[nodiscard]] auto FloorCellCount() const noexcept -> size_t;

private:

  using key_type = cell_id;
  using map_entry_type = std::pair<const key_type, level_cell_type>;
  using cell_allocator_type = custom_allocator<map_entry_type>;
  using collection_allocator_type = custom_allocator<map_entry_type>;
  using collection_type = std::map<key_type, level_cell_type, std::less<key_type>, collection_allocator_type>;

private:

  [[nodiscard]] auto GetType(collection_type::const_iterator cellEntry) const noexcept -> level_cell_type;

private:

  RECT_I m_bounds;
  custom_allocator_state m_cellBuffer;
  cell_allocator_type m_cellAllocator;
  collection_type m_cells;

  cell_id m_leftmostCell;
  cell_id m_rightmostCell;
  cell_id m_topCell;
  cell_id m_bottomCell;

};

auto cell_collection::Enumerate(auto &&visitor) const noexcept -> void
{
  for( const auto& cellEntry : m_cells )
  {
    const auto& [cellId, cellType] = cellEntry;
    visitor(cellId, cellType);
  }
}

inline auto cell_collection::Bounds() const noexcept -> RECT_I
{
  return m_bounds;
}

inline auto cell_collection::FirstColumn() const noexcept -> int
{
  return 0;
}

inline auto cell_collection::EndOfColumns() const noexcept -> int
{
  return cell_id::ColumnCount(m_leftmostCell, m_rightmostCell) + 1;
}

inline auto cell_collection::FirstRow() const noexcept -> int
{
  return 0;
}

inline auto cell_collection::EndOfRows() const noexcept -> int
{
  return cell_id::RowCount(m_topCell, m_bottomCell) + 1;
}

inline auto cell_collection::ColumnTop(int column) const noexcept -> cell_id
{
  auto columnCell = m_leftmostCell.ShiftColumn(column);
  columnCell.SetRow(m_topCell);
  return columnCell;
}

inline auto cell_collection::ColumnBottom(int column) const noexcept -> cell_id
{
  auto columnCell = m_leftmostCell.ShiftColumn(column);
  columnCell.SetRow(m_bottomCell);
  return columnCell;
}

inline auto cell_collection::RowLeft(int row) const noexcept -> cell_id
{
  auto rowCell = m_topCell.ShiftRow(row);
  rowCell.SetColumn(m_leftmostCell);
  return rowCell;
}

inline auto cell_collection::RowRight(int row) const noexcept -> cell_id
{
  auto rowCell = m_topCell.ShiftRow(row);
  rowCell.SetColumn(m_rightmostCell);
  return rowCell;
}

inline auto cell_collection::FloorCellCount() const noexcept -> size_t
{
  auto floorCells = std::ranges::views::filter(m_cells, [](const auto& cellEntry)
  {
    const auto& [cellId, cellType] = cellEntry;
    return cellType == level_cell_type::floor;
  });

  return std::ranges::distance(floorCells);
}
