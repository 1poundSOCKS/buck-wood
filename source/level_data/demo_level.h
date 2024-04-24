#pragma once

#include "framework.h"
#include "level_boundary.h"
#include "valid_cell_collection.h"
#include "level_cell_collection.h"

class demo_level
{

public:

  using points_collection_i = std::list<POINT_2I>;
  using points_collection_f = std::list<POINT_2F>;

  demo_level();

  [[nodiscard]] auto BoundaryPoints() const -> const points_collection_f&;
  [[nodiscard]] auto BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>;

private:

  auto AddArea(std::ranges::input_range auto&& points, POINT_2I position, int offset) noexcept -> void;

private:

  inline static constexpr int m_cellSize = 400;

  level_boundary m_boundary;
  points_collection_i m_boundaryPointsI;
  points_collection_f m_boundaryPointsF;
  winrt::com_ptr<ID2D1Geometry> m_boundaryGeometry;
  level_cell_collection m_cells;

};

auto demo_level::AddArea(std::ranges::input_range auto&& points, POINT_2I position, int offset) noexcept -> void
{
  auto pointIterator = std::begin(m_boundaryPointsI);
  std::advance(pointIterator, offset);

  auto worldPosition = std::ranges::views::transform(points, [this, position](auto point) { return POINT_2I { point.x + position.x * 8, point.y + position.y * 8 }; } );
  auto scaledWorldPosition = std::ranges::views::transform(worldPosition, [this](auto point) { return POINT_2I { point.x * m_cellSize, point.y * m_cellSize }; } );

  std::ranges::copy(scaledWorldPosition, std::inserter(m_boundaryPointsI, pointIterator));
}
