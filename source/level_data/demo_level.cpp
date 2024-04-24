#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level() : m_cells { m_cellSize, m_cellSize }
{
  // auto area1 = std::array {
  //   POINT_2I { -4, -4 },
  //   POINT_2I { 4, -4 },
  //   POINT_2I { 4, 4 },
  //   POINT_2I { -4, 4 }
  // };

  // POINT_2I area1Position = { 0, 0 };

  // AddArea(area1, area1Position, 1);

  // auto area2 = std::array {
  //   POINT_2I { -2, 4 },
  //   POINT_2I { -2, -4 },
  //   POINT_2I { 2, -4 },
  //   POINT_2I { 2, 4 }
  // };

  // POINT_2I area2Position = { 0, -1 };

  // AddArea(area2, area2Position, 1);

  // auto area3 = std::array {
  //   POINT_2I { -4, 4 },
  //   POINT_2I { -4, -4 },
  //   POINT_2I { 4, -4 },
  //   POINT_2I { 4, 4 },
  // };

  // POINT_2I area3Position = { 0, -2 };

  // AddArea(area3, area3Position, 3);

  // std::ranges::transform(m_boundaryPointsI, std::back_inserter(m_boundaryPointsF), [](auto point)
  // {
  //   return POINT_2F { static_cast<float>(point.x), static_cast<float>(point. y) };
  // });

  // m_boundaryGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), m_boundaryPointsF, D2D1_FIGURE_END_CLOSED);
  m_cells.Add(0, 0);
}

[[nodiscard]] auto demo_level::BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>
{
  return m_boundaryGeometry;
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const points_collection_f&
{
  return m_boundaryPointsF;
}
