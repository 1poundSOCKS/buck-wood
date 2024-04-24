#include "pch.h"
#include "level_cell_collection.h"

level_cell_collection::level_cell_collection(int cellWidth, int cellHeight) : m_cellWidth { cellWidth}, m_cellHeight { cellHeight }
{
  auto cellTopRight = POINT_2F { CellBottomRight().x, CellTopLeft().y };
  auto cellBottomLeft = POINT_2F { CellTopLeft().x, CellBottomRight().y };
  auto cellBoundary = std::array { CellTopLeft(), cellTopRight, CellBottomRight(), cellBottomLeft };
  m_cellGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), cellBoundary, D2D1_FIGURE_END_CLOSED);
}

auto level_cell_collection::Add(int x, int y) noexcept -> void
{
  POINT_2F position = CellPosition(x, y);
  winrt::com_ptr<ID2D1TransformedGeometry> geometry = CellGeometry(x, y);
  m_cells.insert({{x, y}, {x, y, position, geometry}});
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
  auto cellX = static_cast<int>(position.x / m_cellWidth);
  auto cellY = static_cast<int>(position.y / m_cellHeight);
  return m_cells.contains({cellX,cellY}) ? cell_type::floor : cell_type::wall;
}
