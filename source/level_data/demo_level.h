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
  [[nodiscard]] auto CellPosition(int x, int y) const noexcept-> POINT_2F;

private:

  inline static constexpr int m_cellSize = 100;

  level_boundary m_boundary;
  std::vector<D2D1_POINT_2F> m_boundaryPoints;
  std::vector<D2D1_POINT_2F> m_targets;
  std::vector<D2D1_POINT_2F> m_asteroids;
  std::vector<D2D1_POINT_2F> m_ductFans;

  constexpr static int m_boundaryWidth { 38 };
  constexpr static int m_boundaryHeight { 24 };

  constexpr static int m_cornerIndent { 4 };

  constexpr static int m_boundaryStartPositionX { -19 };
  constexpr static int m_boundaryStartPositionY { 0 };

  inline static auto m_startBoundaryBuildCommands = {
    boundary_build_command { m_cornerIndent, -m_boundaryHeight / 2 },\
    boundary_build_command { m_boundaryWidth / 2 - m_cornerIndent, -m_cornerIndent },
    boundary_build_command { m_boundaryWidth / 2 - m_cornerIndent, m_cornerIndent },
    boundary_build_command { m_cornerIndent, m_boundaryHeight / 2 },
    boundary_build_command { -m_cornerIndent, m_boundaryHeight / 2 },
    boundary_build_command { -m_boundaryWidth / 2 + m_cornerIndent, m_cornerIndent },
    boundary_build_command { -m_boundaryWidth / 2 + m_cornerIndent, -m_cornerIndent }
  };

  inline static auto m_targetPositions = {
    cell { 0, 0 }
  };

  inline static auto m_asteroidPositions = {
    cell { 3, -6 },
    cell { -3, 6 }
  };

};
