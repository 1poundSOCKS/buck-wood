#pragma once

#include "framework.h"
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

  [[nodiscard]] auto BoundaryPoints() const -> const std::vector<D2D1_POINT_2F>&;
  [[nodiscard]] auto PlayerPosition() const -> POINT_2F;
  [[nodiscard]] auto TargetPositions() const -> const std::vector<D2D1_POINT_2F>&;
  [[nodiscard]] auto AsteroidPositions() const -> const std::vector<D2D1_POINT_2F>&;

private:

  inline static constexpr int m_cellSize = 100;

  level_boundary m_boundary;
  std::vector<D2D1_POINT_2F> m_boundaryPoints;
  std::vector<D2D1_POINT_2F> m_targets;
  std::vector<D2D1_POINT_2F> m_asteroids;
  std::vector<D2D1_POINT_2F> m_ductFans;

  inline static auto m_startBoundaryBuildCommands = {
    boundary_build_command { 36, 0 },
    boundary_build_command { 0, 24 },
    boundary_build_command { -36, 0 }
  };

  inline static auto m_targetPositions = {
    cell { 0, 0 }
  };

  inline static auto m_asteroidPositions = {
    cell { 3, -6 },
    cell { -3, 6 }
  };

};
