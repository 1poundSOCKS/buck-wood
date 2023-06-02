#pragma once

#include "geometry.h"

class shape_iterator
{

public:

  friend class shape_generator;

  shape_iterator() = default;
  using difference_type = std::ptrdiff_t;
  using value_type = game_point;

  auto operator++() -> shape_iterator&;
  auto operator++(int) -> shape_iterator;
  auto operator*() const -> game_point;
  auto operator->() const -> const game_point*;
  auto operator==(const shape_iterator&) const -> bool;
  auto operator<=>(const shape_iterator&) const -> std::strong_ordering;

private:
  
  shape_iterator(const shape_generator* shapeGenerator, int initialAngle);
  auto GetCurrentPoint() const -> game_point;

  const shape_generator* m_shapeGenerator { nullptr };
  int m_currentAngle { 0 };
  game_point m_point { 0, 0 };
};

class shape_generator
{

public:

  friend class shape_iterator;

  shape_generator() = default;
  shape_generator(const shape_generator&) = default;
  shape_generator(shape_generator&&) = default;
  shape_generator(game_rect rect, int sides);

  auto begin() const -> shape_iterator;
  auto end() const -> shape_iterator;

private:

  game_rect m_rect { { 0, 0 }, { 0, 0 } };
  int m_angleIncrement { 0 };
  int m_sides { 0 };

};
