#pragma once

#include "build_command.h"

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
