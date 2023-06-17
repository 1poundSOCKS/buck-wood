#pragma once

#include "geometry.h"
#include "perlin_simplex_noise.h"

class const_shape_iterator
{

public:

  friend class shape_generator;

  const_shape_iterator() = default;
  using difference_type = std::ptrdiff_t;
  using value_type = game_point;

  auto operator++() -> const_shape_iterator&;
  auto operator++(int) -> const_shape_iterator;
  auto operator*() const -> const game_point&;
  auto operator==(const const_shape_iterator&) const -> bool;

private:
  
  const_shape_iterator(const shape_generator* shapeGenerator, int initialAngle);
  auto GetCurrentPoint() const -> game_point;

  const shape_generator* m_shapeGenerator { nullptr };
  int m_currentAngle { 0 };
  game_point m_point { 0, 0 };
};

class shape_generator
{

public:

  friend class const_shape_iterator;

  shape_generator() = default;
  shape_generator(const shape_generator&) = default;
  shape_generator(shape_generator&&) = default;
  shape_generator(float x, float y, float width, float height, int sides);

  auto Left() const -> float;
  auto Top() const -> float;
  auto Right() const -> float;
  auto Bottom() const -> float;

  auto begin() const -> const_shape_iterator;
  auto end() const -> const_shape_iterator;

private:

  float m_x { 0 };
  float m_y { 0 };
  float m_width { 0 };
  float m_height { 0 };
  int m_angleIncrement { 0 };
  int m_sides { 0 };

};

inline auto shape_generator::Left() const -> float
{
  return m_x - m_width / 2;
}

inline auto shape_generator::Top() const -> float
{
  return m_y - m_height / 2;
}

inline auto shape_generator::Right() const -> float
{
  return m_x + m_width / 2;
}

inline auto shape_generator::Bottom() const -> float
{
  return m_y + m_height / 2;
}
