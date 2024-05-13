#include "pch.h"
#include "level_position.h"

level_position::level_position(POINT_2F value) : m_value { value }
{
}

auto level_position::MoveBy(POINT_2F value, const level_cell_collection& cells) -> POINT_2F
{
  auto [column, row] = cells.CellId(m_value);

  auto playerSize = SIZE_F { 90, 90 };

  auto aboveCellId = level_cell_collection::cell_id { column, row - 1 };
  auto belowCellId = level_cell_collection::cell_id { column, row + 1 };
  auto leftCellId = level_cell_collection::cell_id { column - 1, row };
  auto rightCellId = level_cell_collection::cell_id { column + 1, row };
  auto aboveLeftCellId = level_cell_collection::cell_id { column - 1, row - 1};
  auto belowLeftCellId = level_cell_collection::cell_id { column - 1, row + 1};
  auto aboveRightCellId = level_cell_collection::cell_id { column + 1, row - 1};
  auto belowRightCellId = level_cell_collection::cell_id { column + 1, row + 1};

  auto aboveCellRect = ExpandRect(cells.CellRect(aboveCellId), playerSize);
  auto belowCellRect = ExpandRect(cells.CellRect(belowCellId), playerSize);
  auto leftCellRect = ExpandRect(cells.CellRect(leftCellId), playerSize);
  auto rightCellRect = ExpandRect(cells.CellRect(rightCellId), playerSize);
  auto aboveLeftCellRect = ExpandRect(cells.CellRect(aboveLeftCellId), playerSize);
  auto belowLeftCellRect = ExpandRect(cells.CellRect(belowLeftCellId), playerSize);
  auto aboveRightCellRect = ExpandRect(cells.CellRect(aboveRightCellId), playerSize);
  auto belowRightCellRect = ExpandRect(cells.CellRect(belowRightCellId), playerSize);

  auto wallLeft = cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) ? leftCellRect.right : leftCellRect.left;
  auto wallLeftAbove = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) && m_value.y < aboveLeftCellRect.bottom ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallLeftBelow = cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) && m_value.y > belowLeftCellRect.top ? belowLeftCellRect.right : belowLeftCellRect.left;

  auto wallAbove = cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) ? aboveCellRect.bottom : aboveCellRect.top;
  auto wallAboveLeft = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) && m_value.x < aboveLeftCellRect.right ? aboveLeftCellRect.bottom : aboveLeftCellRect.top;
  auto wallAboveRight = cells.IsTypeOf(aboveRightCellId, level_cell_collection::cell_type::wall) && m_value.x > aboveRightCellRect.left ? aboveRightCellRect.bottom : aboveRightCellRect.top;

  auto wallRight = cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) ? rightCellRect.left : rightCellRect.right;
  auto wallRightAbove = cells.IsTypeOf(aboveRightCellId, level_cell_collection::cell_type::wall) && m_value.y < aboveRightCellRect.bottom ? aboveRightCellRect.left : aboveRightCellRect.right;
  auto wallRightBelow = cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall) && m_value.y > belowRightCellRect.top ? belowRightCellRect.left : belowRightCellRect.right;

  auto wallBelow = cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) ? belowCellRect.top : belowCellRect.bottom;
  auto wallBelowLeft = cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) && m_value.x < belowLeftCellRect.right ? belowLeftCellRect.top : belowLeftCellRect.bottom;
  auto wallBelowRight = cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall) && m_value.x > belowRightCellRect.left ? belowRightCellRect.top : belowRightCellRect.bottom;

  wallLeft = std::max({wallLeft, wallLeftAbove, wallLeftBelow});
  wallAbove = std::max({wallAbove, wallAboveLeft, wallAboveRight});
  wallRight = std::min({wallRight, wallRightAbove, wallRightBelow});
  wallBelow = std::min({wallBelow, wallBelowLeft, wallBelowRight});

  auto minXShift = wallLeft  - m_value.x;
  auto maxXShift = wallRight - m_value.x;

  auto minYShift = wallAbove - m_value.y;
  auto maxYShift = wallBelow - m_value.y;

  value.x = std::max(value.x, minXShift);
  value.x = std::min(value.x, maxXShift);

  value.y = std::max(value.y, minYShift);
  value.y = std::min(value.y, maxYShift);

  m_value.x += value.x;
  m_value.y += value.y;

  return m_value;
}

[[nodiscard]] auto level_position::ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F
{
  return { rect.left - size.width, rect.top - size.height, rect.right + size.width, rect.bottom + size.height };
}
