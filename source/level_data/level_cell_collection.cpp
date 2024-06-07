#include "pch.h"
#include "level_cell_collection.h"

level_cell_collection::level_cell_collection(cell_size cellSize) : 
  m_cellSize { cellSize },
  m_cellBuffer { 0, 0 }, m_cellAllocator { m_cellBuffer }, m_cells { m_cellAllocator }
{
  auto cellTopRight = POINT_2F { CellBottomRight().x, CellTopLeft().y };
  auto cellBottomLeft = POINT_2F { CellTopLeft().x, CellBottomRight().y };
  auto cellBoundary = std::array { CellTopLeft(), cellTopRight, CellBottomRight(), cellBottomLeft };
}

auto level_cell_collection::Add(cell_id cellId, level_cell_type cellType) noexcept -> void
{
  m_cells.insert({Key(cellId), cellType});
}

auto level_cell_collection::AddWalls() noexcept -> void
{
  auto minColumn = MinColumn();
  auto maxColumn = MaxColumn();

  auto minRow = MinRow();
  auto maxRow = MaxRow();

  minColumn;maxColumn;minRow;maxRow;

  for( int column = minColumn - 1; column <= maxColumn + 1; ++column )
  {
    for( int row = minRow - 1; row <= maxRow + 1; ++row )
    {
      Add({column, row}, level_cell_type::wall);
    }
  }
}

auto level_cell_collection::CellType(POINT_2F position) const -> level_cell_type
{
  auto id = CellId(position);
  auto key = Key(id);
  auto cell = m_cells.find(key);
  return cell != m_cells.end() ? CellType(cell) : level_cell_type::none;
}

auto level_cell_collection::CellPosition(int x, int y) const noexcept -> POINT_2F
{
  return ToFloat(m_cellSize.CellPosition(cell_id { x, y }));
}

auto level_cell_collection::UpdatePosition(POINT_2F position, POINT_2F distance, SIZE_F objectSize) const noexcept -> POINT_2F
{
  auto cellId = CellId(position);

  auto aboveCellId = cellId.Get(cell_id::relative_position::above);
  auto belowCellId = cellId.Get(cell_id::relative_position::below);
  auto leftCellId = cellId.Get(cell_id::relative_position::left);
  auto rightCellId = cellId.Get(cell_id::relative_position::right);
  auto aboveLeftCellId = cellId.Get(cell_id::relative_position::above_left);
  auto belowLeftCellId = cellId.Get(cell_id::relative_position::below_left);
  auto aboveRightCellId = cellId.Get(cell_id::relative_position::above_right);
  auto belowRightCellId = cellId.Get(cell_id::relative_position::below_right);

  auto aboveCellRect = ExpandRect(m_cellSize.CellRect(aboveCellId), objectSize);
  auto belowCellRect = ExpandRect(m_cellSize.CellRect(belowCellId), objectSize);
  auto leftCellRect = ExpandRect(m_cellSize.CellRect(leftCellId), objectSize);
  auto rightCellRect = ExpandRect(m_cellSize.CellRect(rightCellId), objectSize);
  auto aboveLeftCellRect = ExpandRect(m_cellSize.CellRect(aboveLeftCellId), objectSize);
  auto belowLeftCellRect = ExpandRect(m_cellSize.CellRect(belowLeftCellId), objectSize);
  auto aboveRightCellRect = ExpandRect(m_cellSize.CellRect(aboveRightCellId), objectSize);
  auto belowRightCellRect = ExpandRect(m_cellSize.CellRect(belowRightCellId), objectSize);

  auto wallLeft = IsTypeOf(leftCellId, level_cell_type::wall) ? leftCellRect.right : leftCellRect.left;
  auto wallLeftAbove = IsTypeOf(aboveLeftCellId, level_cell_type::wall) && position.y < aboveLeftCellRect.bottom ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallLeftBelow = IsTypeOf(belowLeftCellId, level_cell_type::wall) && position.y > belowLeftCellRect.top ? belowLeftCellRect.right : belowLeftCellRect.left;

  auto wallAbove = IsTypeOf(aboveCellId, level_cell_type::wall) ? aboveCellRect.bottom : aboveCellRect.top;
  auto wallAboveLeft = IsTypeOf(aboveLeftCellId, level_cell_type::wall) && position.x < aboveLeftCellRect.right ? aboveLeftCellRect.bottom : aboveLeftCellRect.top;
  auto wallAboveRight = IsTypeOf(aboveRightCellId, level_cell_type::wall) && position.x > aboveRightCellRect.left ? aboveRightCellRect.bottom : aboveRightCellRect.top;

  auto wallRight = IsTypeOf(rightCellId, level_cell_type::wall) ? rightCellRect.left : rightCellRect.right;
  auto wallRightAbove = IsTypeOf(aboveRightCellId, level_cell_type::wall) && position.y < aboveRightCellRect.bottom ? aboveRightCellRect.left : aboveRightCellRect.right;
  auto wallRightBelow = IsTypeOf(belowRightCellId, level_cell_type::wall) && position.y > belowRightCellRect.top ? belowRightCellRect.left : belowRightCellRect.right;

  auto wallBelow = IsTypeOf(belowCellId, level_cell_type::wall) ? belowCellRect.top : belowCellRect.bottom;
  auto wallBelowLeft = IsTypeOf(belowLeftCellId, level_cell_type::wall) && position.x < belowLeftCellRect.right ? belowLeftCellRect.top : belowLeftCellRect.bottom;
  auto wallBelowRight = IsTypeOf(belowRightCellId, level_cell_type::wall) && position.x > belowRightCellRect.left ? belowRightCellRect.top : belowRightCellRect.bottom;

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
}

[[nodiscard]] auto level_cell_collection::CellTopLeft() const noexcept -> POINT_2F
{
  auto cellRect = ToFloat(m_cellSize.CellRect({0, 0}));
  return { cellRect.left, cellRect.top };
}

[[nodiscard]] auto level_cell_collection::CellBottomRight() const noexcept -> POINT_2F
{
  auto cellRect = ToFloat(m_cellSize.CellRect({0, 0}));
  return { cellRect.right, cellRect.bottom };
}

[[nodiscard]] auto level_cell_collection::CellRect() const noexcept -> RECT_F
{
  return { CellTopLeft().x, CellTopLeft().y, CellBottomRight().x, CellBottomRight().y };
}

auto level_cell_collection::Key(cell_id cellId) const noexcept -> key_type
{
  auto position = cellId.Position();
  return std::make_tuple(position.x, position.y);
}

auto level_cell_collection::CellType(collection_type::const_iterator cell) const -> level_cell_type
{
  const auto& [celId, cellType] = *cell;
  return cellType;
}

[[nodiscard]] auto level_cell_collection::CellId(POINT_2F position) const -> cell_id
{
  return m_cellSize.CellId(ToInt(position));
}

auto level_cell_collection::CellRect(cell_id cellId) const -> RECT_F
{
  return ToFloat(m_cellSize.CellRect(cellId));
}

auto level_cell_collection::MinColumn() const noexcept -> int
{
  auto cellColumns = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [cellId, cellType] = cellEntry;
    auto [column, row] = cellId;
    return column;
  });

  std::vector<int> columns;
  std::ranges::copy(cellColumns, std::back_inserter(columns));

  auto minElement = std::ranges::min_element(cellColumns);
  return *minElement;
}

auto level_cell_collection::MaxColumn() const noexcept -> int
{
  auto cellColumns = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [cellId, cellType] = cellEntry;
    auto [column, row] = cellId;
    return column;
  });

  auto maxElement = std::ranges::max_element(cellColumns);
  return *maxElement;
}

auto level_cell_collection::MinRow() const noexcept -> int
{
  auto cellRows = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [cellId, cellType] = cellEntry;
    auto [column, row] = cellId;
    return row;
  });

  auto minElement = std::ranges::min_element(cellRows);
  return *minElement;
}

auto level_cell_collection::MaxRow() const noexcept -> int
{
  auto cellRows = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [cellId, cellType] = cellEntry;
    auto [column, row] = cellId;
    return row;
  });

  auto maxElement = std::ranges::max_element(cellRows);
  return *maxElement;
}

auto level_cell_collection::IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool
{
  auto key = Key(cellId);
  auto cellIterator = m_cells.find(key);
  return cellIterator == std::end(m_cells) ? false : cellIterator->second == cellType;
}

[[nodiscard]] auto level_cell_collection::ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F
{
  return { rect.left - size.width, rect.top - size.height, rect.right + size.width, rect.bottom + size.height };
}

auto level_cell_collection::ExpandRect(RECT_I rect, SIZE_F size) -> RECT_F
{
  auto rectF = ToFloat(rect);
  return { rectF.left - size.width, rectF.top - size.height, rectF.right + size.width, rectF.bottom + size.height };
}
