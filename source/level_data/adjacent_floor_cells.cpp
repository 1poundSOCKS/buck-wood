#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
  auto cellIds = std::ranges::views::transform(m_cellPositions, [this](auto position)
  {
    return m_cellId.Get(position);
  });

  auto floorCellIds = std::ranges::views::filter(cellIds, [this](auto cellId)
  {
    return m_cells.IsTypeOf(cellId, level_cell_type::floor);
  });

  auto floorCellCount = std::ranges::distance(floorCellIds);

  m_cellIds.reserve(floorCellCount);
  std::ranges::copy(floorCellIds, std::back_inserter(m_cellIds));
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  return m_cellIds.size();
}

auto adjacent_floor_cells::operator[](size_t index) -> cell_id
{
  return m_cellIds[index];
}
