#include "pch.h"
#include "cell_collection.h"

cell_collection::cell_collection() noexcept : m_cellBuffer { 0, 0 }, m_cellAllocator { m_cellBuffer }, m_cells { m_cellAllocator }
{
}

auto cell_collection::Set(cell_id cellId, level_cell_type cellType) noexcept -> void
{
  m_cells[cellId] = cellType;
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
