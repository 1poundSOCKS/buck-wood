#pragma once

#include "game_point.h"
#include "level_boundary.h"

class demo_level
{

public:

  struct cell
  {
    int x { 0 };
    int y { 0 };
  };

  demo_level();

  [[nodiscard]] auto BoundaryPoints() const -> const std::vector<game_point>&;
  [[nodiscard]] auto TargetPositions() const -> const std::vector<game_point>&;
  [[nodiscard]] auto AsteroidPositions() const -> const std::vector<game_point>&;
  [[nodiscard]] auto DuctFanPositions() const -> const std::vector<game_point>&;

private:

  inline static constexpr int m_cellSize = 100;

  level_boundary m_boundary;
  std::vector<game_point> m_boundaryPoints;
  std::vector<game_point> m_targets;
  std::vector<game_point> m_asteroids;
  std::vector<game_point> m_ductFans;

  inline static auto m_startBoundaryBuildCommands = {
    boundary_build_command { 8, 0 },
    boundary_build_command { 8, 8 },
    boundary_build_command { 0, 8 },
    boundary_build_command { -8, 8 },
    boundary_build_command { -8, 0 },
    boundary_build_command { -8, -8 },
    boundary_build_command { 0, -8 }
  };

  inline static auto m_targetPositions = {
    cell { -6, -6 },
    cell { -6, 6 },
    cell { 6, -6 },
    cell { 6, 6 }
  };

  inline static auto m_ductFanPositions = {
    cell { 0, 8 },
    cell { 8, 0 },
    cell { 0, -8 },
    cell { -8, 0 }
  };

  inline static auto m_asteroidPositions = {
    cell { 3, -6 },
    cell { -3, 6 }
  };

};
