#pragma once

#include "framework.h"
#include "level_boundary.h"
#include "valid_cell_collection.h"

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
  [[nodiscard]] auto BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>;

private:

  inline static constexpr int m_cellSize = 400;

  level_boundary m_boundary;
  std::vector<D2D1_POINT_2F> m_boundaryPoints;
  std::vector<D2D1_POINT_2F> m_targets;
  std::vector<D2D1_POINT_2F> m_asteroids;
  std::vector<D2D1_POINT_2F> m_ductFans;

  constexpr static int m_boundaryWidth { 8 };
  constexpr static int m_boundaryHeight { 6 };

  constexpr static int m_cornerIndent { 1 };

  constexpr static int m_boundaryStartPositionX { -4 };
  constexpr static int m_boundaryStartPositionY { -3 };

  inline static auto m_startBoundaryBuildCommands = {
    boundary_build_command { m_boundaryWidth, 0 },\
    boundary_build_command { 0, m_boundaryHeight },\
    boundary_build_command { -m_boundaryWidth, 0 }
  };

  winrt::com_ptr<ID2D1Geometry> m_boundaryGeometry;

};
