#include "pch.h"
#include "level_cell_collection.h"

level_cell_collection::level_cell_collection(int cellWidth, int cellHeight) : m_cellWidth { cellWidth}, m_cellHeight { cellHeight }
{
  auto cellTopRight = POINT_2F { CellBottomRight().x, CellTopLeft().y };
  auto cellBottomLeft = POINT_2F { CellTopLeft().x, CellBottomRight().y };
  auto cellBoundary = std::array { CellTopLeft(), cellTopRight, CellBottomRight(), cellBottomLeft };
  m_cellGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), cellBoundary, D2D1_FIGURE_END_CLOSED);
}

auto level_cell_collection::Add(valid_cell::cell_type cellType, int x, int y) noexcept -> void
{
  POINT_2F position = CellPosition(x, y);
  winrt::com_ptr<ID2D1TransformedGeometry> geometry = CellGeometry(x, y);
  m_cells.insert({{x, y}, {cellType, x, y, position, geometry}});
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
      Add(valid_cell::cell_type::wall, column, row);
    }
  }
}

auto level_cell_collection::CellPosition(int x, int y) const noexcept -> POINT_2F
{
  return POINT_2F(static_cast<float>(x * m_cellWidth), static_cast<float>(y * m_cellHeight));
}

auto level_cell_collection::CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  auto cellPosition = CellPosition(x, y);
  auto cellTranslation = SIZE_F { cellPosition.x, cellPosition.y };
  auto cellTransform = D2D1::Matrix3x2F::Translation(cellTranslation);
  return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), m_cellGeometry.get(), cellTransform);
}

[[nodiscard]] auto level_cell_collection::CellTopLeft() const noexcept -> POINT_2F
{
  return { static_cast<float>(-m_cellWidth / 2), static_cast<float>(-m_cellHeight / 2) };
}

[[nodiscard]] auto level_cell_collection::CellBottomRight() const noexcept -> POINT_2F
{
  return { static_cast<float>(m_cellWidth / 2), static_cast<float>(m_cellHeight / 2) };
}

[[nodiscard]] auto level_cell_collection::CellRect() const noexcept -> RECT_F
{
  return { CellTopLeft().x, CellTopLeft().y, CellBottomRight().x, CellBottomRight().y };
}

[[nodiscard]] auto level_cell_collection::CellType(POINT_2F position) const -> cell_type
{
  const auto& [column, row] = CellId(position);
  auto cell = m_cells.find({column, row});
  return cell != m_cells.end() ? CellType(cell) : cell_type::empty;
}

auto level_cell_collection::CellType(collection_type::const_iterator cell) const -> cell_type
{
  const auto& [key, value] = *cell;

  switch( value.Type() )
  {
    case valid_cell::cell_type::floor:
      return cell_type::floor;
    case valid_cell::cell_type::wall:
      return cell_type::wall;
    default:
      return cell_type::empty;
  }
}

[[nodiscard]] auto level_cell_collection::CellId(POINT_2F position) const -> cell_id
{
  auto halfCellWidth = m_cellWidth / 2;
  auto halfCellHeight = m_cellHeight / 2;
  auto cellColumn = ( position.x < 0) ? static_cast<int>((position.x - halfCellWidth) / m_cellWidth) : static_cast<int>((position.x + halfCellWidth) / m_cellWidth);
  auto cellRow = ( position.y < 0) ? static_cast<int>((position.y - halfCellHeight ) / m_cellHeight) : static_cast<int>((position.y + halfCellHeight ) / m_cellHeight);
  return { cellColumn, cellRow };
}

auto level_cell_collection::MinColumn() const noexcept -> int
{
  auto cellColumns = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [key, value] = cellEntry;
    return value.X();
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
    const auto& [key, value] = cellEntry;
    return value.X();
  });

  auto maxElement = std::ranges::max_element(cellColumns);
  return *maxElement;
}

auto level_cell_collection::MinRow() const noexcept -> int
{
  auto cellRows = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [key, value] = cellEntry;
    return value.Y();
  });

  auto minElement = std::ranges::min_element(cellRows);
  return *minElement;
}

auto level_cell_collection::MaxRow() const noexcept -> int
{
  auto cellRows = std::ranges::views::transform(m_cells, [](const auto& cellEntry) -> int
  {
    const auto& [key, value] = cellEntry;
    return value.Y();
  });

  auto maxElement = std::ranges::max_element(cellRows);
  return *maxElement;
}

auto level_cell_collection::IsTypeOf(cell_id cellId, cell_type cellType) const noexcept -> bool
{
  auto cellIterator = m_cells.find(cellId);

  if( cellIterator == std::end(m_cells) )
  {
    return false;
  }
  else
  {
    switch( cellIterator->second.Type() )
    {

      case valid_cell::cell_type::wall:
        return cellType == cell_type::wall;

      case valid_cell::cell_type::floor:
        return cellType == cell_type::floor;
        
      default:
        return false;

    }
  }
}
