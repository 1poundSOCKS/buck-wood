#pragma once

class level_grid_cell
{
public:

  level_grid_cell() = default;
  level_grid_cell(const level_grid_cell&) = default;
  level_grid_cell(float x, float y, float width, float height);

  auto Position() const -> D2D1_POINT_2F;
  auto Width() const -> float;
  auto Height() const -> float;
  auto Left() const -> float;
  auto Top() const -> float;
  auto Right() const -> float;
  auto Bottom() const -> float;

  auto Resize(float xRatio, float yRatio) -> void;

private:

  D2D1_POINT_2F m_position { 0, 0 };
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
