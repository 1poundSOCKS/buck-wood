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
class shape_iterator
{

public:

  friend class shape_generator<noise>;

  shape_iterator() = default;
  using difference_type = std::ptrdiff_t;
  using value_type = game_point;

  auto operator++() -> shape_iterator&;
  auto operator++(int) -> shape_iterator;
  auto operator*() const -> game_point;
  auto operator==(const shape_iterator&) const -> bool;
  auto operator<=>(const shape_iterator&) const -> std::strong_ordering;

private:
  
  shape_iterator(const shape_generator<noise>* shapeGenerator, int initialAngle);
  auto GetCurrentPoint() const -> game_point;

  const shape_generator<noise>* m_shapeGenerator { nullptr };
  int m_currentAngle { 0 };
  game_point m_point { 0, 0 };
};

template <typename noise>
class shape_generator
{

public:

  friend class shape_iterator<noise>;

  shape_generator() = default;
  shape_generator(const shape_generator<noise>&) = default;
  shape_generator(shape_generator<noise>&&) = default;
  shape_generator(game_rect rect, int sides);

  auto begin() const -> shape_iterator<noise>;
  auto end() const -> shape_iterator<noise>;

private:

  game_rect m_rect { { 0, 0 }, { 0, 0 } };
  int m_angleIncrement { 0 };
  int m_sides { 0 };
  noise m_noise;

};


// static_assert(std::ranges::input_range<shape_generator<clean>>);

// static_assert(std::input_iterator<shape_iterator<clean>>);

template <typename noise>
shape_iterator<typename noise>::shape_iterator(const shape_generator<noise>* shapeGenerator, int initialAngle) : 
  m_shapeGenerator(shapeGenerator), m_currentAngle(initialAngle)
{
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
}

template <typename noise>
auto shape_iterator<typename noise>::operator++() -> shape_iterator&
{
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return *this;
}

template <typename noise>
auto shape_iterator<typename noise>::operator++(int) -> shape_iterator
{
  shape_iterator tmp = *this;
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return tmp;
}

template <typename noise>
auto shape_iterator<typename noise>::operator==(const shape_iterator& i) const -> bool
{
  return m_currentAngle == i.m_currentAngle;
}

template <typename noise>
auto shape_iterator<typename noise>::operator<=>(const shape_iterator &i) const -> std::strong_ordering
{
  if( m_currentAngle < i.m_currentAngle )
  {
      return std::strong_ordering::less;
  }
  else if( m_currentAngle > i.m_currentAngle )
  {
    return std::strong_ordering::greater;
  }
  else
  {
    return std::strong_ordering::equal;
  }
}

template <typename noise>
auto shape_iterator<typename noise>::operator*() const -> game_point
{
  return m_point;
}

template <typename noise>
auto shape_iterator<typename noise>::GetCurrentPoint() const -> game_point
{
  auto radiusX = ( m_shapeGenerator->m_rect.bottomRight.x - m_shapeGenerator->m_rect.topLeft.x ) / 2;
  auto radiusY = ( m_shapeGenerator->m_rect.bottomRight.y - m_shapeGenerator->m_rect.topLeft.y ) / 2;

  auto angleInRadians = DEGTORAD(m_currentAngle);
  auto cx = radiusX * sin(angleInRadians);
  auto cy = radiusY * cos(angleInRadians);

  auto x = m_shapeGenerator->m_rect.topLeft.x + radiusX;
  auto y = m_shapeGenerator->m_rect.bottomRight.y + radiusY;

  // auto noise = ( psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy)) + 5 ) / 6;
  auto noise = ( m_shapeGenerator->m_noise.GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy)) + 5 ) / 6;
  
  return {x + cx * noise, y + cy * noise};
}

template <typename noise>
shape_generator<typename noise>::shape_generator(game_rect rect, int sides) : m_rect(rect), m_sides(sides), m_angleIncrement(360 / sides)
{
}

template <typename noise>
auto shape_generator<typename noise>::begin() const -> shape_iterator<noise>
{
  return shape_iterator(this, 0);
}

template <typename noise>
auto shape_generator<typename noise>::end() const -> shape_iterator<noise>
{
  return shape_iterator(this, 360);
}
