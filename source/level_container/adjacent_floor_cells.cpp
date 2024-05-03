#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, level_cell_collection::cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  auto adjacentCellIdsatOrigin = std::array
  {
    level_cell_collection::cell_id { 0, -1 },
    level_cell_collection::cell_id { 1, 0 },
    level_cell_collection::cell_id { 0, 1 },
    level_cell_collection::cell_id { -1, 0 }
  };

  auto adjacentCellIds = std::ranges::views::transform(adjacentCellIdsatOrigin, [this](auto cellId)
  {
    const auto& [column, row] = m_cellId;
    const auto& [columnShift, rowShift] = cellId;
    return level_cell_collection::cell_id { column + columnShift, row + rowShift };
  });

  auto validAdjacentFloorCellIds = std::ranges::views::filter(adjacentCellIds, [this](auto cellId)
  {
    return m_cells.IsTypeOf(cellId, level_cell_collection::cell_type::floor);
  });

  return std::ranges::distance(validAdjacentFloorCellIds);
}

auto adjacent_floor_cells::operator[](size_t index) -> const valid_cell &
{
  auto adjacentCellIdsatOrigin = std::array
  {
    level_cell_collection::cell_id { 0, -1 },
    level_cell_collection::cell_id { 1, 0 },
    level_cell_collection::cell_id { 0, 1 },
    level_cell_collection::cell_id { -1, 0 }
  };

  auto adjacentCellIds = std::ranges::views::transform(adjacentCellIdsatOrigin, [this](auto cellId)
  {
    const auto& [column, row] = m_cellId;
    const auto& [columnShift, rowShift] = cellId;
    return level_cell_collection::cell_id { column + columnShift, row + rowShift };
  });

  auto validAdjacentFloorCellIds = std::ranges::views::filter(adjacentCellIds, [this](auto cellId)
  {
    return m_cells.IsTypeOf(cellId, level_cell_collection::cell_type::floor);
  });

  auto cellIdIterator = std::begin(validAdjacentFloorCellIds);
  std::advance(cellIdIterator, index);
  return m_cells.Get(*cellIdIterator);
}
