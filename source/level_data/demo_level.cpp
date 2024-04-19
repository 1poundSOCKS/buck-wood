#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"
#include "collisions/geometry_containment.h"

demo_level::demo_level()
{
  std::vector<geometry_builder::point> boundaryPoints;

  m_boundary.AddGeometry(m_startBoundaryBuildCommands);
  
  m_boundary.Build(m_boundaryStartPositionX, m_boundaryStartPositionY, std::back_inserter(boundaryPoints));

  std::ranges::transform(boundaryPoints, std::back_inserter(m_boundaryPoints), [](geometry_builder::point point) -> D2D1_POINT_2F
  {
    const auto [x, y] = point;
    return { static_cast<float>(x * m_cellSize), static_cast<float>(y * m_cellSize) };
  });

  std::ranges::transform(m_targetPositions, std::back_inserter(m_targets), [](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * m_cellSize);
    auto y = static_cast<float>(targetCell.y * m_cellSize);
    return D2D1_POINT_2F { x, y };
  });

#ifdef ENABLE_ASTEROIDS
  std::ranges::transform(m_asteroidPositions, std::back_inserter(m_asteroids), [](const cell& asteroidCell)
  {
    auto x = static_cast<float>(asteroidCell.x * m_cellSize);
    auto y = static_cast<float>(asteroidCell.y * m_cellSize);
    return D2D1_POINT_2F { x, y };
  });
#endif

  m_boundaryGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), m_boundaryPoints, D2D1_FIGURE_END_CLOSED);
  m_boundaryRect = direct2d::GetGeometryBounds(m_boundaryGeometry.get());

  auto cellTopRight = POINT_2F { CellBottomRight().x, CellTopLeft().y };
  auto cellBottomLeft = POINT_2F { CellTopLeft().x, CellBottomRight().y };
  auto cellBoundary = std::array { CellTopLeft(), cellTopRight, CellBottomRight(), cellBottomLeft };
  m_cellGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), cellBoundary, D2D1_FIGURE_END_CLOSED);

  for( auto x = MinCellX(); x <= MaxCellX(); ++x )
  {
    for( auto y = MinCellY(); y <= MaxCellY(); ++y )
    {
      auto geometry = CellGeometry(x, y);      
      m_validCellGeometries.emplace_back(CellIsValid(geometry), geometry);
    }
  }
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const std::vector<D2D1_POINT_2F>&
{
  return m_boundaryPoints;
}

[[nodiscard]] auto demo_level::PlayerPosition() const -> POINT_2F
{
  return CellPosition(0, 0);
}

constexpr [[nodiscard]] auto demo_level::CellWidth() noexcept-> float
{
  return static_cast<float>(m_cellSize);
}

constexpr [[nodiscard]] auto demo_level::CellHeight() noexcept-> float
{
  return static_cast<float>(m_cellSize);
}

[[nodiscard]] auto demo_level::CellPosition(int x, int y) const noexcept-> POINT_2F
{
  return { static_cast<float>(x * CellWidth()), static_cast<float>(y * CellHeight()) };
}

constexpr [[nodiscard]] auto demo_level::CellTopLeft() noexcept -> POINT_2F
{
  return { -CellWidth() / 2, -CellHeight() / 2 };
}

constexpr [[nodiscard]] auto demo_level::CellBottomRight() noexcept -> POINT_2F
{
  return { CellWidth() / 2, CellHeight() / 2 };
}

constexpr [[nodiscard]] auto demo_level::CellRect() noexcept -> RECT_F
{
  return { CellTopLeft().x, CellTopLeft().y, CellBottomRight().x, CellBottomRight().y };
}

[[nodiscard]] auto demo_level::MinCellX() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.left / CellWidth());
}

[[nodiscard]] auto demo_level::MinCellY() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.top / CellHeight());
}

[[nodiscard]] auto demo_level::MaxCellX() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.right / CellWidth());
}

[[nodiscard]] auto demo_level::MaxCellY() const noexcept -> int
{
  return static_cast<int>(m_boundaryRect.bottom / CellHeight());
}

[[nodiscard]] auto demo_level::CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  auto cellPosition = CellPosition(x, y);
  auto cellTranslation = SIZE_F { cellPosition.x, cellPosition.y };
  auto cellTransform = D2D1::Matrix3x2F::Translation(cellTranslation);
  return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), m_cellGeometry.get(), cellTransform);
}

[[nodiscard]] auto demo_level::CellIsValid(winrt::com_ptr<ID2D1TransformedGeometry> geometry) const noexcept -> bool
{
  geometry_containment containmentCheck;
  return containmentCheck.IsContained(m_boundaryGeometry.get(), geometry.get());
}

[[nodiscard]] auto demo_level::ValidCellGeometries() const -> const valid_cell_collection&
{
  return m_validCellGeometries;
}
