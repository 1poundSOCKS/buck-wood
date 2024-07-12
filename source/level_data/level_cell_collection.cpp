#include "pch.h"
#include "level_cell_collection.h"

level_cell_collection::level_cell_collection(cell_size cellSize, RECT_I bounds) : 
  m_cellSize { cellSize }, m_cells { bounds }
{
}

auto level_cell_collection::Get(cell_id cellId) const -> level_cell_item
{
  auto type = m_cells.GetType(cellId);
  auto position = m_cellSize.CellPosition(cellId);
  return { cellId, type, position };
}

auto level_cell_collection::Set(cell_id cellId, level_cell_type cellType) noexcept -> void
{
  m_cells.Set(cellId, cellType);
}

auto level_cell_collection::CellSize() const -> cell_size
{
  return m_cellSize;
}

auto level_cell_collection::CellType(POINT_2F position) const -> level_cell_type
{
  auto cellId = CellId(position);
  return m_cells.GetType(cellId);
}

auto level_cell_collection::CellPosition(cell_id cellId) const noexcept -> POINT_2F
{
  return ToFloat(m_cellSize.CellPosition(cellId));
}

auto level_cell_collection::IsUnoccupied(cell_id cellId) const noexcept -> bool
{
  return true;
}

auto level_cell_collection::SetWall(cell_id cellId, cell_id::relative_position position) noexcept -> void
{
  auto wallCellId = cellId.Get(position);
  
  if( m_cells.IsTypeOf(wallCellId, level_cell_type::none) )
  {
    m_cells.Set(wallCellId, level_cell_type::wall);
  }
}

[[nodiscard]] auto level_cell_collection::CellId(POINT_2F position) const -> cell_id
{
  return m_cellSize.CellId(ToInt(position));
}

auto level_cell_collection::CellRect(cell_id cellId) const -> RECT_F
{
  return ToFloat(m_cellSize.CellRect(cellId));
}

auto level_cell_collection::IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool
{
  auto levelCellType = m_cells.GetType(cellId);
  return cellType == levelCellType;
}
