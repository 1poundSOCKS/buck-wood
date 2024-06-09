#pragma once

#include "framework.h"
#include "cell_collection.h"
#include "cell_size.h"
#include "level_cell_item.h"
// #include "level_cell_movement.h"

class level_cell_collection
{

public:

  level_cell_collection(cell_size cellSize);

  [[nodiscard]] auto Get(cell_id cellId) const -> level_cell_item;

  auto Set(cell_id cellId, level_cell_type cellType) noexcept -> void;

  [[nodiscard]] auto CellType(POINT_2F position) const -> level_cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;
  [[nodiscard]] auto CellRect(cell_id cellId) const -> RECT_F;

  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  [[nodiscard]] auto CellPosition(cell_id cellId) const noexcept -> POINT_2F;

  // [[nodiscard]] auto UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F;

  auto Enumerate(auto&& visitor) const noexcept -> void;

private:

  auto SetWall(cell_id cellId, cell_id::relative_position position) noexcept -> void;

private:

  cell_size m_cellSize;
  cell_collection m_cells;
  // level_cell_movement m_cellMovement;

};

auto level_cell_collection::Enumerate(auto &&visitor) const noexcept -> void
{
  m_cells.Enumerate([this,visitor](auto cellId, auto cellType) -> void
  {
    auto cellPosition = m_cellSize.CellPosition(cellId);
    level_cell_item cellItem { cellId, cellType, cellPosition };
    visitor(cellItem);
  });
}
