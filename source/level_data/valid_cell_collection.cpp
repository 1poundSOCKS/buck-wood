#include "pch.h"
#include "framework.h"
#include "valid_cell_collection.h"
#include "collisions/geometry_containment.h"

auto valid_cell_collection::Load(ID2D1Geometry *boundary, float cellWidth, float cellHeight) -> void
{
  m_boundaryRect = direct2d::GetGeometryBounds(boundary);
  m_cellWidth = cellWidth;
  m_cellHeight = cellHeight;

  auto cellTopRight = POINT_2F { CellBottomRight().x, CellTopLeft().y };
  auto cellBottomLeft = POINT_2F { CellTopLeft().x, CellBottomRight().y };
  auto cellBoundary = std::array { CellTopLeft(), cellTopRight, CellBottomRight(), cellBottomLeft };
  m_cellGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), cellBoundary, D2D1_FIGURE_END_CLOSED);

  for( auto x = MinCellX(); x <= MaxCellX(); ++x )
  {
    for( auto y = MinCellY(); y <= MaxCellY(); ++y )
    {
      auto geometry = CellGeometry(x, y);
      if( CellIsValid(boundary, geometry.get()) )
      {
        EmplaceBack(x, y, CellPosition(x, y), geometry);
      }
    }
  }
}

[[nodiscard]] auto valid_cell_collection::CellPosition(int x, int y) const noexcept-> POINT_2F
{
  return { static_cast<float>(x * m_cellWidth), static_cast<float>(y * m_cellHeight) };
}

[[nodiscard]] auto valid_cell_collection::CellTopLeft() const noexcept -> POINT_2F
{
  return { -m_cellWidth / 2, -m_cellHeight / 2 };
}

[[nodiscard]] auto valid_cell_collection::CellBottomRight() const noexcept -> POINT_2F
{
  return { m_cellWidth / 2, m_cellHeight / 2 };
}

[[nodiscard]] auto valid_cell_collection::CellRect() const noexcept -> RECT_F
{
  return { CellTopLeft().x, CellTopLeft().y, CellBottomRight().x, CellBottomRight().y };
}

[[nodiscard]] auto valid_cell_collection::MinCellX() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.left / m_cellWidth);
}

[[nodiscard]] auto valid_cell_collection::MinCellY() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.top / m_cellHeight);
}

[[nodiscard]] auto valid_cell_collection::MaxCellX() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.right / m_cellWidth);
}

[[nodiscard]] auto valid_cell_collection::MaxCellY() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.bottom / m_cellHeight);
}

[[nodiscard]] auto valid_cell_collection::CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  auto cellPosition = CellPosition(x, y);
  auto cellTranslation = SIZE_F { cellPosition.x, cellPosition.y };
  auto cellTransform = D2D1::Matrix3x2F::Translation(cellTranslation);
  return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), m_cellGeometry.get(), cellTransform);
}

[[nodiscard]] auto valid_cell_collection::CellIsValid(ID2D1Geometry *boundary, ID2D1Geometry* cell) const noexcept -> bool
{
  geometry_containment containmentCheck;
  return containmentCheck.IsContained(boundary, cell);
}
