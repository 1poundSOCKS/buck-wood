#include "pch.h"
#include "demo_level.h"

demo_level::demo_level(int cellWidth, int cellHeight)
{
  std::vector<geometry_builder::point> boundaryPoints;

  m_boundary.AddGeometry(m_startBoundaryBuildCommands);
  m_boundary.AddGeometry(m_joinBoundaryBuildCommands);
  m_boundary.AddGeometry(m_endBoundaryBuildCommands);
  m_boundary.Build(std::back_inserter(boundaryPoints));

  std::ranges::transform(boundaryPoints, std::back_inserter(m_boundaryPoints), [cellWidth, cellHeight](geometry_builder::point point) -> game_point
  {
    const auto [x, y] = point;
    return { static_cast<float>(x * cellWidth) - cellWidth / 2, static_cast<float>(y * cellHeight) - cellHeight / 2  };
  });

  std::ranges::transform(m_targetPositions, std::back_inserter(m_targets), [cellWidth, cellHeight](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * cellWidth);
    auto y = static_cast<float>(targetCell.y * cellHeight);
    return game_point { x, y };
  });

#ifdef INCLUDE_ASTEROIDS
  std::ranges::transform(m_asteroidPositions, std::back_inserter(m_asteroids), [cellWidth, cellHeight](const cell& asteroidCell)
  {
    auto x = static_cast<float>(asteroidCell.x * cellWidth);
    auto y = static_cast<float>(asteroidCell.y * cellHeight);
    return game_point { x, y };
  });
#endif

  std::ranges::transform(m_ductFanPositions, std::back_inserter(m_ductFans), [cellWidth, cellHeight](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * cellWidth);
    auto y = static_cast<float>(targetCell.y * cellHeight);
    return game_point { x, y };
  });
}

[[nodiscard]] auto demo_level::Boundary() const -> const std::vector<game_point>&
{
  return m_boundaryPoints;
}

[[nodiscard]] auto demo_level::Targets() const -> const std::vector<game_point>&
{
  return m_targets;
}

[[nodiscard]] auto demo_level::Asteroids() const -> const std::vector<game_point>&
{
  return m_asteroids;
}

[[nodiscard]] auto demo_level::DuctFans() const -> const std::vector<game_point>&
{
  return m_ductFans;
}