#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, level_cell_collection::cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  size_t adjacentFloorCellCount = 0;

  AdjacentFloorCellIdView(m_adjacentCellIdsatOrigin, m_cells, m_cellId, [&adjacentFloorCellCount](const auto& cellId)
  {
    ++adjacentFloorCellCount;
  });

  return adjacentFloorCellCount;
}

auto adjacent_floor_cells::operator[](size_t index) -> level_cell_collection::cell_id
{
  level_cell_collection::cell_id adjacentCellId { m_cellId };

  AdjacentFloorCellIdView(m_adjacentCellIdsatOrigin, m_cells, m_cellId, [&adjacentCellId, &index](const auto& cellId)
  {
    if( index == 0 )
    {
      adjacentCellId = cellId;
    }

    --index;
  });

  return adjacentCellId;
}
