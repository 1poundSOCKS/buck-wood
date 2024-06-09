#pragma once

#include "level_cell_collection.h"
#include "cell_size.h"

class level_cell_movement
{

public:

  level_cell_movement();
  level_cell_movement(std::shared_ptr<level_cell_collection> cells, cell_size cellSize);

  [[nodiscard]] auto UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F;

private:

  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;
  [[nodiscard]] auto CellRect(cell_id cellId) const noexcept -> RECT_F;

  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;
  static [[nodiscard]] auto ExpandRect(RECT_I rect, SIZE_F size) -> RECT_F;

private:

  std::shared_ptr<level_cell_collection> m_cells;
  cell_size m_cellSize;

};
