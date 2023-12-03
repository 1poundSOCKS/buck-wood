#pragma once

#include "geometry.h"

class build_command
{

public:

  build_command(int cx, int cy) : m_cx { cx }, m_cy { cy }
  {
  }

  [[nodiscard]] auto cx() const -> int
  {
    return m_cx;
  }

  [[nodiscard]] auto cy() const -> int
  {
    return m_cy;
  }

private:

  int m_cx { 0 };
  int m_cy { 0 };

};

class geometry_builder
{

public:

  geometry_builder(int startX, int startY, int cellWidth, int cellHeight) : 
    m_startX { startX }, m_startY { startY }, m_cellWidth { cellWidth }, m_cellHeight { cellHeight }
  {
  }

  auto Run(std::ranges::input_range auto&& commands, auto output) -> void
  {
    float x = static_cast<float>(m_startX * m_cellWidth) - m_cellWidth / 2;
    float y = static_cast<float>(m_startY * m_cellHeight) - m_cellHeight / 2;

    output = game_point { x, y };

    auto view = commands | std::ranges::views::transform([this, &x,&y](const build_command& buildCommand) -> game_point
    {
      x += buildCommand.cx() * m_cellWidth;
      y += buildCommand.cy() * m_cellHeight;
      return { x, y };
    });

    std::ranges::copy(view, output);
  }

private:

  int m_startX { 0 };
  int m_startY { 0 };
  int m_cellWidth { 1 };
  int m_cellHeight { 1 };

};

class demo_level
{

public:

  struct cell { int x { 0 }; int y { 0 }; };

  demo_level(int cellWidth, int cellHeight);

  [[nodiscard]] auto Boundary() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Targets() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Asteroids() const -> const std::vector<game_point>&;
  [[nodiscard]] auto DuctFans() const -> const std::vector<game_point>&;

private:

  inline static int m_left { -2 };
  inline static int m_top { -2 };

  std::vector<game_point> m_boundary;
  std::vector<game_point> m_targets;
  std::vector<game_point> m_asteroids;
  std::vector<game_point> m_ductFans;

  inline static auto m_boundaryBuildCommands = {
    build_command { 1, -1 },
    build_command { 1, 0 },
    build_command { 1, 3 },
    build_command { 1, 0 },
    build_command { 0, 4 },
    build_command { -1, 0 },
    build_command { 0, 3 },
    build_command { 1, 1 },
    build_command { 2, 0 },
    build_command { 0, 1 },
    build_command { -5, 0 },
    build_command { 0, -1 },
    build_command { 1, 0 },
    build_command { 0, -4 },
    build_command { -2, 0 },
    build_command { -1, -1 },
    build_command { 0, -2 }
  };

  inline static auto m_targetPositions = {
    cell { -2, -1 },
    cell { 0, 1 },
    cell { 0, 5 }
  };

  inline static auto m_asteroidPositions = {
    cell { -1, 1 },
    cell { 1, 0 },
    cell { -2, -2 }
  };

  inline static auto m_ductFanPositions = {
    cell { 0, 4 }
  };

};
