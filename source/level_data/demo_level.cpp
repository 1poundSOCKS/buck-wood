#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level() : m_validCells { std::make_shared<valid_cell_collection>() }
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

  m_validCells->Load(m_boundaryGeometry.get(), static_cast<float>(m_cellSize), static_cast<float>(m_cellSize));
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const std::vector<D2D1_POINT_2F>&
{
  return m_boundaryPoints;
}

[[nodiscard]] auto demo_level::PlayerPosition() const -> POINT_2F
{
  return m_validCells->CellPosition(0, 0);
}

[[nodiscard]] auto demo_level::ValidCellCollection() const -> std::shared_ptr<valid_cell_collection>
{
  return m_validCells;
}
