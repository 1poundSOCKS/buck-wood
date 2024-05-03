#pragma once

#include "level_cell_collection.h"

[[nodiscard]] auto AdjacentFloorCellIdView(std::ranges::input_range auto&& cellIds, const level_cell_collection& cells, level_cell_collection::cell_id originCellId, auto&& callable) -> void
{
  auto adjacentCellIds = std::ranges::views::transform(cellIds, [originCellId](auto cellId)
  {
    const auto& [column, row] = originCellId;
    const auto& [columnShift, rowShift] = cellId;
    return level_cell_collection::cell_id { column + columnShift, row + rowShift };
  });

  auto adjacentFloorCellIds = std::ranges::views::filter(adjacentCellIds, [&cells](auto cellId)
  {
    return cells.IsTypeOf(cellId, level_cell_collection::cell_type::floor);
  });

  for( const auto& adjacentCellId : adjacentFloorCellIds )
  {
    callable(adjacentCellId);
  }
}

class adjacent_floor_cells
{

public:

  adjacent_floor_cells(const level_cell_collection& cells, level_cell_collection::cell_id cellId);

  [[nodiscard]] auto Count() const noexcept -> size_t;
  [[nodiscard]] auto operator[](size_t index) -> level_cell_collection::cell_id;

private:

  const level_cell_collection& m_cells;
  const level_cell_collection::cell_id m_cellId;

};
