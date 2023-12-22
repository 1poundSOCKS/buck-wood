#include "pch.h"
#include "demo_level.h"

demo_level::demo_level()
{
  std::vector<geometry_builder::point> boundaryPoints;

  m_boundary.AddGeometry(m_startBoundaryBuildCommands);
  
  m_boundary.Build(-4, -12, std::back_inserter(boundaryPoints));

  std::ranges::transform(boundaryPoints, std::back_inserter(m_boundaryPoints), [](geometry_builder::point point) -> game_point
  {
    const auto [x, y] = point;
    return { static_cast<float>(x * m_cellSize), static_cast<float>(y * m_cellSize) };
  });

  std::ranges::transform(m_targetPositions, std::back_inserter(m_targets), [](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * m_cellSize);
    auto y = static_cast<float>(targetCell.y * m_cellSize);
    return game_point { x, y };
  });

  std::ranges::transform(m_asteroidPositions, std::back_inserter(m_asteroids), [](const cell& asteroidCell)
  {
    auto x = static_cast<float>(asteroidCell.x * m_cellSize);
    auto y = static_cast<float>(asteroidCell.y * m_cellSize);
    return game_point { x, y };
  });

  std::ranges::transform(m_ductFanPositions, std::back_inserter(m_ductFans), [](const cell& targetCell)
  {
    auto x = static_cast<float>(targetCell.x * m_cellSize);
    auto y = static_cast<float>(targetCell.y * m_cellSize);
    return game_point { x, y };
  });
}

[[nodiscard]] auto demo_level::BoundaryPoints() const -> const std::vector<game_point>&
{
  return m_boundaryPoints;
}

[[nodiscard]] auto demo_level::TargetPositions() const -> const std::vector<game_point>&
{
  return m_targets;
}

[[nodiscard]] auto demo_level::AsteroidPositions() const -> const std::vector<game_point>&
{
  return m_asteroids;
}

[[nodiscard]] auto demo_level::DuctFanPositions() const -> const std::vector<game_point>&
{
  return m_ductFans;
}
