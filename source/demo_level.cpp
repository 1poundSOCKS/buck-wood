#include "pch.h"
#include "demo_level.h"

demo_level::demo_level(int cellWidth, int cellHeight)
{
  geometry_builder geometryBuilder { m_left, m_top, cellWidth, cellHeight };
  geometryBuilder.Run(m_buildCommands, std::back_inserter(m_boundary));

  std::ranges::transform(m_targetPositions, std::back_inserter(m_targets), [cellWidth, cellHeight](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * cellWidth);
    auto y = static_cast<float>(targetCell.y * cellHeight);
    return game_point { x, y };
  });
}

[[nodiscard]] auto demo_level::Boundary() const -> const std::vector<game_point>&
{
  return m_boundary;
}

[[nodiscard]] auto demo_level::Targets() const -> const std::vector<game_point>&
{
  return m_targets;
}
