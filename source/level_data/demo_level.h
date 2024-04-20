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
  [[nodiscard]] auto PlayerPosition() const -> POINT_2F;

  constexpr static [[nodiscard]] auto CellWidth() noexcept-> float;
  constexpr static [[nodiscard]] auto CellHeight() noexcept-> float;
  [[nodiscard]] auto CellPosition(int x, int y) const noexcept-> POINT_2F;
  constexpr static [[nodiscard]] auto CellTopLeft() noexcept -> POINT_2F;
  constexpr static [[nodiscard]] auto CellBottomRight() noexcept -> POINT_2F;
  constexpr static [[nodiscard]] auto CellRect() noexcept -> RECT_F;

  [[nodiscard]] auto MinCellX() const noexcept -> int;
  [[nodiscard]] auto MinCellY() const noexcept -> int;
  [[nodiscard]] auto MaxCellX() const noexcept -> int;
  [[nodiscard]] auto MaxCellY() const noexcept -> int;

  [[nodiscard]] auto CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>;
  [[nodiscard]] auto CellIsValid(winrt::com_ptr<ID2D1TransformedGeometry> geometry) const noexcept -> bool;
  auto ValidCellCollection() const -> std::shared_ptr<valid_cell_collection>;

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

  winrt::com_ptr<ID2D1Geometry> m_boundaryGeometry;
  RECT_F m_boundaryRect { 0, 0, 0, 0 };
  winrt::com_ptr<ID2D1Geometry> m_cellGeometry;
  std::shared_ptr<valid_cell_collection> m_validCells;

};
