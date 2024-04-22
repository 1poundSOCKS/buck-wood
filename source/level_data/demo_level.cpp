#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  auto area1 = std::array {
    POINT_2F { static_cast<float>(-4 * m_cellSize), static_cast<float>(-4 * m_cellSize) },
    POINT_2F { static_cast<float>(4 * m_cellSize), static_cast<float>(-4 * m_cellSize) },
    POINT_2F { static_cast<float>(4 * m_cellSize), static_cast<float>(4 * m_cellSize) },
    POINT_2F { static_cast<float>(-4 * m_cellSize), static_cast<float>(4 * m_cellSize) }
  };

  auto area2 = std::array {
    POINT_2F {static_cast<float>(-2 * m_cellSize), static_cast<float>(-4 * m_cellSize) },
    POINT_2F {static_cast<float>(-2 * m_cellSize), static_cast<float>(-8 * m_cellSize) },
    POINT_2F {static_cast<float>(2 * m_cellSize), static_cast<float>(-8 * m_cellSize) },
    POINT_2F {static_cast<float>(2 * m_cellSize), static_cast<float>(-4 * m_cellSize) }
  };

  std::ranges::copy(area1, std::back_inserter(m_boundaryPoints));

  auto pointIterator = std::begin(m_boundaryPoints);
  std::advance(pointIterator, 1);
  std::ranges::copy(area2, std::inserter(m_boundaryPoints, pointIterator));
  // m_boundaryPoints.insert(pointIterator, {static_cast<float>(-2 * m_cellSize), static_cast<float>(-4 * m_cellSize)});
  // m_boundaryPoints.insert(pointIterator, {static_cast<float>(-2 * m_cellSize), static_cast<float>(-8 * m_cellSize)});
  // m_boundaryPoints.insert(pointIterator, {static_cast<float>(2 * m_cellSize), static_cast<float>(-8 * m_cellSize)});
  // m_boundaryPoints.insert(pointIterator, {static_cast<float>(2 * m_cellSize), static_cast<float>(-4 * m_cellSize)});

  m_boundaryGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), m_boundaryPoints, D2D1_FIGURE_END_CLOSED);
}

[[nodiscard]] auto demo_level::BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>
{
  return m_boundaryGeometry;
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const points_collection&
{
  return m_boundaryPoints;
}
