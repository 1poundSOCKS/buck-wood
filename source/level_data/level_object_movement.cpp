#include "pch.h"
#include "level_object_movement.h"

level_object_movement::level_object_movement(std::shared_ptr<level_cell_collection> cells) : m_cells{ cells }
{ 
}

auto level_object_movement::UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F
{
  auto cellSize = m_cells->CellSize();

  auto cellId = cellSize.CellId(ToInt(position));

  auto aboveCellId = cellId.Get(cell_id::relative_position::above);
  auto belowCellId = cellId.Get(cell_id::relative_position::below);
  auto leftCellId = cellId.Get(cell_id::relative_position::left);
  auto rightCellId = cellId.Get(cell_id::relative_position::right);
  auto aboveLeftCellId = cellId.Get(cell_id::relative_position::above_left);
  auto belowLeftCellId = cellId.Get(cell_id::relative_position::below_left);
  auto aboveRightCellId = cellId.Get(cell_id::relative_position::above_right);
  auto belowRightCellId = cellId.Get(cell_id::relative_position::below_right);

  auto aboveCellRect = ExpandRect(cellSize.CellRect(aboveCellId), objectSize);
  auto belowCellRect = ExpandRect(cellSize.CellRect(belowCellId), objectSize);
  auto leftCellRect = ExpandRect(cellSize.CellRect(leftCellId), objectSize);
  auto rightCellRect = ExpandRect(cellSize.CellRect(rightCellId), objectSize);
  auto aboveLeftCellRect = ExpandRect(cellSize.CellRect(aboveLeftCellId), objectSize);
  auto belowLeftCellRect = ExpandRect(cellSize.CellRect(belowLeftCellId), objectSize);
  auto aboveRightCellRect = ExpandRect(cellSize.CellRect(aboveRightCellId), objectSize);
  auto belowRightCellRect = ExpandRect(cellSize.CellRect(belowRightCellId), objectSize);

  auto wallLeft = m_cells->IsTypeOf(leftCellId, level_cell_type::wall) ? leftCellRect.right : leftCellRect.left;
  auto wallLeftAbove = m_cells->IsTypeOf(aboveLeftCellId, level_cell_type::wall) && position.y < aboveLeftCellRect.bottom ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallLeftBelow = m_cells->IsTypeOf(belowLeftCellId, level_cell_type::wall) && position.y > belowLeftCellRect.top ? belowLeftCellRect.right : belowLeftCellRect.left;

  auto wallAbove = m_cells->IsTypeOf(aboveCellId, level_cell_type::wall) ? aboveCellRect.bottom : aboveCellRect.top;
  auto wallAboveLeft = m_cells->IsTypeOf(aboveLeftCellId, level_cell_type::wall) && position.x < aboveLeftCellRect.right ? aboveLeftCellRect.bottom : aboveLeftCellRect.top;
  auto wallAboveRight = m_cells->IsTypeOf(aboveRightCellId, level_cell_type::wall) && position.x > aboveRightCellRect.left ? aboveRightCellRect.bottom : aboveRightCellRect.top;

  auto wallRight = m_cells->IsTypeOf(rightCellId, level_cell_type::wall) ? rightCellRect.left : rightCellRect.right;
  auto wallRightAbove = m_cells->IsTypeOf(aboveRightCellId, level_cell_type::wall) && position.y < aboveRightCellRect.bottom ? aboveRightCellRect.left : aboveRightCellRect.right;
  auto wallRightBelow = m_cells->IsTypeOf(belowRightCellId, level_cell_type::wall) && position.y > belowRightCellRect.top ? belowRightCellRect.left : belowRightCellRect.right;

  auto wallBelow = m_cells->IsTypeOf(belowCellId, level_cell_type::wall) ? belowCellRect.top : belowCellRect.bottom;
  auto wallBelowLeft = m_cells->IsTypeOf(belowLeftCellId, level_cell_type::wall) && position.x < belowLeftCellRect.right ? belowLeftCellRect.top : belowLeftCellRect.bottom;
  auto wallBelowRight = m_cells->IsTypeOf(belowRightCellId, level_cell_type::wall) && position.x > belowRightCellRect.left ? belowRightCellRect.top : belowRightCellRect.bottom;

  wallLeft = std::max({wallLeft, wallLeftAbove, wallLeftBelow});
  wallAbove = std::max({wallAbove, wallAboveLeft, wallAboveRight});
  wallRight = std::min({wallRight, wallRightAbove, wallRightBelow});
  wallBelow = std::min({wallBelow, wallBelowLeft, wallBelowRight});

  auto minXShift = wallLeft  - position.x;
  auto maxXShift = wallRight - position.x;

  auto minYShift = wallAbove - position.y;
  auto maxYShift = wallBelow - position.y;

  auto maxAdjustedDistance = POINT_2F { std::max(distance.x, minXShift), std::max(distance.y, minYShift) };
  auto adjustedDistance = POINT_2F { std::min(maxAdjustedDistance.x, maxXShift), std::min(maxAdjustedDistance.y, maxYShift) };

  return { position.x + adjustedDistance.x, position.y + adjustedDistance.y };
  // return { position.x + distance.x, position.y + distance.y };
}

[[nodiscard]] auto level_object_movement::CellTopLeft() const noexcept -> POINT_2F
{
  auto cellRect = ToFloat(m_cells->CellSize().CellRect({0, 0}));
  return { cellRect.left, cellRect.top };
}

[[nodiscard]] auto level_object_movement::CellBottomRight() const noexcept -> POINT_2F
{
  auto cellRect = ToFloat(m_cells->CellSize().CellRect({0, 0}));
  return { cellRect.right, cellRect.bottom };
}

[[nodiscard]] auto level_object_movement::CellRect() const noexcept -> RECT_F
{
  return { CellTopLeft().x, CellTopLeft().y, CellBottomRight().x, CellBottomRight().y };
}

[[nodiscard]] auto level_object_movement::CellRect(cell_id cellId) const noexcept -> RECT_F
{
  return ToFloat(m_cells->CellSize().CellRect(cellId));
}

[[nodiscard]] auto level_object_movement::ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F
{
  return { rect.left - size.width, rect.top - size.height, rect.right + size.width, rect.bottom + size.height };
}

auto level_object_movement::ExpandRect(RECT_I rect, SIZE_F size) -> RECT_F
{
  auto rectF = ToFloat(rect);
  return { rectF.left - size.width, rectF.top - size.height, rectF.right + size.width, rectF.bottom + size.height };
}
