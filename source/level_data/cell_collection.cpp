#include "pch.h"
#include "cell_collection.h"

cell_collection::cell_collection(RECT_I bounds) noexcept : m_bounds { bounds }, m_cellBuffer { 0, 0 }, m_cellAllocator { m_cellBuffer }, m_cells { m_cellAllocator }
{
}

auto cell_collection::Set(cell_id cellId, level_cell_type cellType) noexcept -> void
{
  m_cells[cellId] = cellType;

  m_leftmostCell = cellId.IsLeftOf(m_leftmostCell) ? cellId : m_leftmostCell;
  m_rightmostCell = cellId.IsRightOf(m_rightmostCell) ? cellId : m_rightmostCell;
  m_topCell = cellId.IsAbove(m_topCell) ? cellId : m_topCell;
  m_bottomCell = cellId.IsBelow(m_bottomCell) ? cellId : m_bottomCell;
}

auto cell_collection::Reset() noexcept -> void
{
  m_cells.clear();
}

auto cell_collection::GetType(cell_id cellId) const noexcept -> level_cell_type
{
  auto cellEntry = m_cells.find(cellId);
  return cellEntry == std::end(m_cells) ? level_cell_type::none : GetType(cellEntry);
}

auto cell_collection::IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool
{
  return GetType(cellId) == cellType;
}

[[nodiscard]] auto cell_collection::GetType(collection_type::const_iterator cellEntry) const noexcept -> level_cell_type
{
  const auto& [id,type] = *cellEntry;
  return type;
}
