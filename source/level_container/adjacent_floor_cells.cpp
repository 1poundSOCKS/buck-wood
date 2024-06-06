#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
  auto aboveId = m_cellId.Get(cell_id::relative_position::above);
  auto rightId = m_cellId.Get(cell_id::relative_position::right);
  auto belowId = m_cellId.Get(cell_id::relative_position::below);
  auto leftId = m_cellId.Get(cell_id::relative_position::left);

  bool aboveIsFloor = m_cells.IsTypeOf(aboveId, level_cell_type::floor);
  bool rightIsFloor = m_cells.IsTypeOf(rightId, level_cell_type::floor);
  bool belowIsFloor = m_cells.IsTypeOf(belowId, level_cell_type::floor);
  bool leftIsFloor = m_cells.IsTypeOf(leftId, level_cell_type::floor);

  auto adjacentFloorCellCount = 0;
  adjacentFloorCellCount += aboveIsFloor ? 1 : 0;
  adjacentFloorCellCount += rightIsFloor ? 1 : 0;
  adjacentFloorCellCount += belowIsFloor ? 1 : 0;
  adjacentFloorCellCount += leftIsFloor ? 1 : 0;

  m_adjacentCellIds.reserve(adjacentFloorCellCount);

  if( aboveIsFloor ) m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::above));
  if( rightIsFloor ) m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::right));
  if( belowIsFloor ) m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::below));
  if( leftIsFloor ) m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::left));
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  return m_adjacentCellIds.size();
}

auto adjacent_floor_cells::operator[](size_t index) -> cell_id
{
  return m_adjacentCellIds[index];
}
