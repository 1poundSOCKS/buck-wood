#pragma once

#include "geometry.h"

class level_grid_cell
{
public:

  level_grid_cell() = default;
  level_grid_cell(const level_grid_cell&) = default;
  level_grid_cell(float x, float y, float width, float height);

  auto Position() const -> const game_point&;
  auto Width() const -> float;
  auto Height() const -> float;
  auto Left() const -> float;
  auto Top() const -> float;
  auto Right() const -> float;
  auto Bottom() const -> float;

  auto Resize(float xRatio, float yRatio) -> void;

private:

  game_point m_position { 0, 0 };
  float m_width { 0 };
  float m_height { 0 };

};

inline auto level_grid_cell::Width() const -> float
{
  return m_width;
}

inline auto level_grid_cell::Height() const -> float
{
  return m_height;
}
