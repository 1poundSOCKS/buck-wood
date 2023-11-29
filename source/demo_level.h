#pragma once

#include "geometry.h"
#include "level_grid.h"

class build_command
{

public:

  enum class move_direction { none, left, right, up, down };

  build_command(move_direction moveDirection, int count) : m_moveDirection { moveDirection }, m_count { count }
  {
  }

  [[nodiscard]] auto MoveDirection() const -> move_direction
  {
    return m_moveDirection;
  }

  [[nodiscard]] auto Count() const -> int
  {
    return m_count;
  }

private:

  move_direction m_moveDirection { move_direction::none };
  int m_count { 0 };

};

class geometry_builder
{

public:

  geometry_builder(int startX, int startY, int cellWidth, int cellHeight) : 
    m_startX { startX }, m_startY { startY }, m_cellWidth { cellWidth }, m_cellHeight { cellHeight }
  {
  }

  auto Run(auto&& commands, auto output) -> void
  {
    float x = static_cast<float>(m_startX * m_cellWidth);
    float y = static_cast<float>(m_startY * m_cellHeight);

    output = game_point { x, y };

    auto view = commands | std::ranges::views::transform([this, &x,&y](const build_command& buildCommand) -> game_point
    {
      switch( buildCommand.MoveDirection() )
      {
        case build_command::move_direction::left:
          x -= buildCommand.Count() * m_cellWidth;
          break;
        case build_command::move_direction::right:
          x += buildCommand.Count() * m_cellWidth;
          break;
        case build_command::move_direction::up:
          y -= buildCommand.Count() * m_cellHeight;
          break;
        case build_command::move_direction::down:        
          y += buildCommand.Count() * m_cellHeight;
          break;
      }

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

  demo_level(const level_grid& levelGrid);

  [[nodiscard]] auto Boundary() const -> const std::vector<game_point>&;

private:

  std::vector<game_point> m_boundary;

  inline static auto m_buildCommands = std::array {
    build_command { build_command::move_direction::right, 4 },
    build_command { build_command::move_direction::up, 2 },
    build_command { build_command::move_direction::right, 2 },
    build_command { build_command::move_direction::down, 5 },
    build_command { build_command::move_direction::right, 4 },
    build_command { build_command::move_direction::down, 10 },
    build_command { build_command::move_direction::left, 10 }
  };

};
