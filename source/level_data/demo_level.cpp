#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  auto area1 = std::array {
    POINT_2I { -4, -4 },
    POINT_2I { 4, -4 },
    POINT_2I { 4, 4 },
    POINT_2I { -4, 4 }
  };

  POINT_2I area1Position = { 0, 0 };

  auto area2 = std::array {
    POINT_2I { -2, 4 },
    POINT_2I { -2, -4 },
    POINT_2I { 2, -4 },
    POINT_2I { 2, 4 }
  };

  POINT_2I area2Position = { 0, -1 };

  auto area3 = std::array {
    POINT_2I { -4, 4 },
    POINT_2I { -4, -4 },
    POINT_2I { 4, -4 },
    POINT_2I { 4, 4 },
  };

  POINT_2I area3Position = { 0, -2 };

  auto area1Transform = std::ranges::views::transform(area1, [this](auto point) { return POINT_2I { point.x * m_cellSize, point.y * m_cellSize }; } );

  std::ranges::copy(area1Transform, std::back_inserter(m_boundaryPointsI));

  auto area2Transform1 = std::ranges::views::transform(area2, [this](auto point) { return POINT_2I { point.x, point.y - 8 }; } );
  auto area2Transform2 = std::ranges::views::transform(area2Transform1, [this](auto point) { return POINT_2I { point.x * m_cellSize, point.y * m_cellSize }; } );
  
  auto pointIterator = std::begin(m_boundaryPointsI);
  std::advance(pointIterator, 1);
  std::ranges::copy(area2Transform2, std::inserter(m_boundaryPointsI, pointIterator));

  auto area3Transform1 = std::ranges::views::transform(area3, [this](auto point) { return POINT_2I { point.x , point.y - 16}; } );
  auto area3Transform2 = std::ranges::views::transform(area3Transform1, [this](auto point) { return POINT_2I { point.x * m_cellSize, point.y * m_cellSize }; } );

  pointIterator = std::begin(m_boundaryPointsI);
  std::advance(pointIterator, 3);
  std::ranges::copy(area3Transform2, std::inserter(m_boundaryPointsI, pointIterator));

  std::ranges::transform(m_boundaryPointsI, std::back_inserter(m_boundaryPointsF), [](auto point)
  {
    return POINT_2F { static_cast<float>(point.x), static_cast<float>(point. y) };
  });

  m_boundaryGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), m_boundaryPointsF, D2D1_FIGURE_END_CLOSED);
}

[[nodiscard]] auto demo_level::BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>
{
  return m_boundaryGeometry;
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const points_collection_f&
{
  return m_boundaryPointsF;
}
