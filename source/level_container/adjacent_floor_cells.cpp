#include "pch.h"
#include "adjacent_floor_cells.h"

adjacent_floor_cells::adjacent_floor_cells(const level_cell_collection &cells, cell_id cellId) : m_cells { cells }, m_cellId { cellId }
{
  m_adjacentCellIds.reserve(4);
  m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::above));
  m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::right));
  m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::below));
  m_adjacentCellIds.emplace_back(m_cellId.Get(cell_id::relative_position::left));
}

auto adjacent_floor_cells::Count() const noexcept -> size_t
{
  return m_adjacentCellIds.size();
}

auto adjacent_floor_cells::operator[](size_t index) -> cell_id
{
  return m_adjacentCellIds[index];
}
