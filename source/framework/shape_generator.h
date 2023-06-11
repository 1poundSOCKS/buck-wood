#pragma once

#include "geometry.h"
#include "perlin_simplex_noise.h"

template <typename noise>
class shape_generator;

class clean_shape
{
public:
  auto GetNoise(float x, float y) const -> float
  {
    return 0;
  }
};

class noisy_shape
{
public:
  auto GetNoise(float x, float y) const -> float
  {
    return psn::GetNoise(x, y );
  }
};

template <typename noise>
class const_shape_iterator
{

public:

  friend class shape_generator<noise>;

  const_shape_iterator() = default;
  using difference_type = std::ptrdiff_t;
  using value_type = game_point;

  auto operator++() -> const_shape_iterator&;
  auto operator++(int) -> const_shape_iterator;
  auto operator*() const -> const game_point&;
  auto operator==(const const_shape_iterator&) const -> bool;

private:
  
  const_shape_iterator(const shape_generator<noise>* shapeGenerator, int initialAngle);
  auto GetCurrentPoint() const -> game_point;

  const shape_generator<noise>* m_shapeGenerator { nullptr };
  int m_currentAngle { 0 };
  game_point m_point { 0, 0 };
};

template <typename noise>
class shape_generator
{

public:

  friend class const_shape_iterator<noise>;

  shape_generator() = default;
  shape_generator(const shape_generator<noise>&) = default;
  shape_generator(shape_generator<noise>&&) = default;
  shape_generator(game_rect rect, int sides);

  auto begin() const -> const_shape_iterator<noise>;
  auto end() const -> const_shape_iterator<noise>;

private:

  game_rect m_rect { { 0, 0 }, { 0, 0 } };
  int m_angleIncrement { 0 };
  int m_sides { 0 };
  noise m_noise;

};

template <typename noise>
const_shape_iterator<typename noise>::const_shape_iterator(const shape_generator<noise>* shapeGenerator, int initialAngle) : 
  m_shapeGenerator(shapeGenerator), m_currentAngle(initialAngle)
{
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
}

template <typename noise>
auto const_shape_iterator<typename noise>::operator++() -> const_shape_iterator&
{
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return *this;
}

template <typename noise>
auto const_shape_iterator<typename noise>::operator++(int) -> const_shape_iterator
{
  const_shape_iterator tmp = *this;
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return tmp;
}

template <typename noise>
auto const_shape_iterator<typename noise>::operator==(const const_shape_iterator& i) const -> bool
{
  return m_currentAngle == i.m_currentAngle;
}

template <typename noise>
auto const_shape_iterator<typename noise>::operator*() const -> const game_point&
{
  return m_point;
}

template <typename noise>
auto const_shape_iterator<typename noise>::GetCurrentPoint() const -> game_point
{
  auto centrePoint = m_shapeGenerator->m_rect.CentrePoint();

  auto radiusX = centrePoint.x - m_shapeGenerator->m_rect.topLeft.x;
  auto radiusY = centrePoint.y - m_shapeGenerator->m_rect.topLeft.y;

  auto angleInRadians = DEGTORAD(m_currentAngle);

  auto cx = radiusX * sin(angleInRadians);
  auto cy = radiusY * cos(angleInRadians);

  auto x = centrePoint.x + cx;
  auto y = centrePoint.y - cy;

  auto noise = ( m_shapeGenerator->m_noise.GetNoise(static_cast<float>(x), static_cast<float>(y)) + 5 ) / 6;
  
  return { x + noise, y + noise };
}

template <typename noise>
shape_generator<typename noise>::shape_generator(game_rect rect, int sides) : m_rect(rect), m_sides(sides), m_angleIncrement(360 / sides)
{
}

template <typename noise>
auto shape_generator<typename noise>::begin() const -> const_shape_iterator<noise>
{
  return const_shape_iterator(this, 0);
}

template <typename noise>
auto shape_generator<typename noise>::end() const -> const_shape_iterator<noise>
{
  return const_shape_iterator(this, 360);
}
