#pragma once

#include "game_point.h"

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
    auto view = commands | std::ranges::views::transform([this](const build_command& buildCommand) -> point
    {
      m_x += buildCommand.cx() * m_cellWidth;
      m_y += buildCommand.cy() * m_cellHeight;
      return { m_x, m_y };
    });

    std::ranges::copy(view, output);
  }

  auto Run(const build_command& command, auto output) -> void
  {
    m_x += command.cx() * m_cellWidth;
    m_y += command.cy() * m_cellHeight;
    output = { m_x, m_y };
  }

private:

  int m_x { 0 };
  int m_y { 0  };
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

  [[nodiscard]] auto Type() const -> type
  {
    return m_type;
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

  auto Build(auto outputPoints) -> void
  {
    Build(std::begin(m_builds), outputPoints);
  }

private:

  auto Build(build_type_collection::const_iterator currentBuild, auto outputPoints) -> void
  {
    for( const auto& inputCommand : *currentBuild )
    {
      switch( inputCommand.Type() )
      {
        case boundary_build_command::type::portal:
          if( std::next(currentBuild) != std::end(m_builds) )
          {
            Build(std::next(currentBuild), outputPoints);
          }
          break;
        default:
          m_builder.Run(inputCommand, outputPoints);
          break;
      }
    }
  }

  build_type_collection m_builds;
  geometry_builder m_builder;

};

class demo_level
{

public:

  struct cell
  {
    int x { 0 };
    int y { 0 };
  };

  demo_level();

  [[nodiscard]] auto Boundary() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Targets() const -> const std::vector<game_point>&;
  [[nodiscard]] auto Asteroids() const -> const std::vector<game_point>&;
  [[nodiscard]] auto DuctFans() const -> const std::vector<game_point>&;

private:

  inline static constexpr int m_cellSize = 150;  

  level_boundary m_boundary;
  std::vector<game_point> m_boundaryPoints;
  std::vector<game_point> m_targets;
  std::vector<game_point> m_asteroids;
  std::vector<game_point> m_ductFans;

  inline static auto m_startBoundaryBuildCommands = {
    boundary_build_command { -1, -3 },
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

};
