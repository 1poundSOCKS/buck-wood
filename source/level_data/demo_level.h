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
    boundary_build_command { -2, -6 },
    boundary_build_command { 4, 0 },
    boundary_build_command { 4, 4 },
    boundary_build_command { 0, 4 },
    boundary_build_command { -4, 4 },
    boundary_build_command { -4, 0, boundary_build_command::type::portal },
    boundary_build_command { -4, -4 },
    boundary_build_command { 0, -4 }
  };

  inline static auto m_joinBoundaryBuildCommands = {
    boundary_build_command { 0, 12 },
    boundary_build_command { -4, 0, boundary_build_command::type::portal },
    boundary_build_command { 0, -12 }
  };

  inline static auto m_endBoundaryBuildCommands = {
    boundary_build_command { 4, 4 },
    boundary_build_command { 0, 4 },
    boundary_build_command { -4, 4 },
    boundary_build_command { -4, 0 },
    boundary_build_command { -4, -4 },
    boundary_build_command { 0, -4 },
    boundary_build_command { 4, -4 }
  };

  inline static auto m_targetPositions = {
    cell { 0, 24 }
  };

  inline static auto m_ductFanPositions = {
    cell { 0, 12 }
  };

  inline static auto m_asteroidPositions = {
    cell { 0, 1 },
    cell { -3, 3 },
    cell { 3, 26 },
    cell { -1, 22 }
  };

};
