#include "pch.h"
#include "level_grid_cell.h"

level_grid_cell::level_grid_cell(float x, float y, float width, float height) : m_position { x, y }, m_width { width }, m_height { height }
{
}

auto level_grid_cell::Position() const -> const game_point&
{
  return m_position;
}

auto level_grid_cell::Left() const -> float
{
  return m_position.x - m_width / 2;
}

auto level_grid_cell::Top() const -> float
{
  return m_position.y - m_height / 2;
}

auto level_grid_cell::Right() const -> float
{
  return m_position.x + m_width / 2;
}

auto level_grid_cell::Bottom() const -> float
{
  return m_position.y + m_height / 2;
}

auto level_grid_cell::Resize(float xRatio, float yRatio) -> void
{
  m_width *= xRatio;
  m_height *= yRatio;
}

