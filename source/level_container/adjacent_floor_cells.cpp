#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
  auto cellPositions = std::array {
    cell_id::relative_position::above,
    cell_id::relative_position::right,
    cell_id::relative_position::below,
    cell_id::relative_position::left
  };

  auto cellIds = std::ranges::views::transform(cellPositions, [this](auto position)
  {
    return m_cellId.Get(position);
  });

  auto floorCellIds = std::ranges::views::filter(cellIds, [this](auto cellId)
  {
    return m_cells.IsTypeOf(cellId, level_cell_type::floor);
  });

  auto floorCellCount = std::ranges::distance(floorCellIds);

  m_adjacentCellIds.reserve(floorCellCount);
  std::ranges::copy(floorCellIds, std::back_inserter(m_adjacentCellIds));
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  return m_adjacentCellIds.size();
}

auto adjacent_floor_cells::operator[](size_t index) -> cell_id
{
  return m_adjacentCellIds[index];
}
