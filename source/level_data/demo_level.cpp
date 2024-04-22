#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

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

  m_boundaryGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), m_boundaryPoints, D2D1_FIGURE_END_CLOSED);
}

[[nodiscard]] auto demo_level::BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>
{
  return m_boundaryGeometry;
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const std::vector<D2D1_POINT_2F>&
{
  return m_boundaryPoints;
}
