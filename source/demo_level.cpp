#include "pch.h"
#include "demo_level.h"

demo_level::demo_level(const level_grid& levelGrid)
{
  geometry_builder geometryBuilder { levelGrid.LeftColumn() - 1, levelGrid.TopRow() - 1, levelGrid.ColumnWidth(), levelGrid.RowHeight() };
  geometryBuilder.Run(m_buildCommands, std::back_inserter(m_boundary));
}

[[nodiscard]] auto demo_level::Boundary() const -> const std::vector<game_point>&
{
  return m_boundary;
}
