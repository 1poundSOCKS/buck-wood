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

  using point = std::tuple<int, int>;

  auto Run(std::ranges::input_range auto&& commands, auto output) -> void
  {
    int x = 0, y = 0;

    auto view = commands | std::ranges::views::transform([this, &x,&y](const build_command& buildCommand) -> point
    {
      x += buildCommand.cx() * m_cellWidth;
      y += buildCommand.cy() * m_cellHeight;
      return { x, y };
    });

    std::ranges::copy(view, output);
  }

private:

  int m_cellWidth { 1 };
  int m_cellHeight { 1 };

};

class boundary_build_command : public build_command
{

public:

  enum class type { normal, portal };

  boundary_build_command(int cx, int cy) : build_command { cx, cy }
  {
  }

  boundary_build_command(int cx, int cy, type commandType) : build_command { cx, cy }, m_type { commandType }
  {
  }

private:

  type m_type { type::normal };

};

class level_boundary
{

public:

  using build_type = std::vector<boundary_build_command>;
  using build_type_collection = std::list<build_type>;
  using points_type = std::vector<geometry_builder::point>;

  auto AddGeometry(std::ranges::input_range auto&& commands) -> void
  {
    m_builds.emplace_back(commands);    
  }

  auto Build(auto&& outputPoints) -> void
  {
    geometry_builder geometryBuilder;
    geometryBuilder.Run(m_builds.front(), outputPoints);
  }

private:

  build_type_collection m_builds;

};

class demo_level
{

public:

  struct cell
  {
    int x { 0 };
    int y { 0 };
  };

  demo_level(int cellWidth, int cellHeight);

  [[nodiscard]] auto Boundary() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Targets() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Asteroids() const -> const std::vector<game_point>&;
  [[nodiscard]] auto DuctFans() const -> const std::vector<game_point>&;

private:

  level_boundary m_boundary;
  std::vector<game_point> m_boundaryPoints;
  std::vector<game_point> m_targets;
  std::vector<game_point> m_asteroids;
  std::vector<game_point> m_ductFans;

  inline static auto m_boundaryBuildCommands = {
    boundary_build_command { 0, -1 },
    boundary_build_command { 1, 0 },
    boundary_build_command { 1, 1 },
    boundary_build_command { 0, 1 },
    boundary_build_command { -1, 1 },
    boundary_build_command { -1, 0, boundary_build_command::type::portal },
    boundary_build_command { -1, -1 },
    boundary_build_command { 0, -1 }
  };

  inline static auto m_targetPositions = {
    cell { 0, 1 }
  };

};
