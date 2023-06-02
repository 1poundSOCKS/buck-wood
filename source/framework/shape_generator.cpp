#include "pch.h"
#include "shape_generator.h"
#include "perlin_simplex_noise.h"

static_assert(std::ranges::input_range<shape_generator>);
static_assert(std::input_iterator<shape_iterator>);

shape_iterator::shape_iterator(const shape_generator* shapeGenerator, int initialAngle) : 
  m_shapeGenerator(shapeGenerator), m_currentAngle(initialAngle)
{
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
}

auto shape_iterator::operator++() -> shape_iterator&
{
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return *this;
}

auto shape_iterator::operator++(int) -> shape_iterator
{
  shape_iterator tmp = *this;
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return tmp;
}

auto shape_iterator::operator==(const shape_iterator& i) const -> bool
{
  return m_currentAngle == i.m_currentAngle;
}

auto shape_iterator::operator<=>(const shape_iterator &i) const -> std::strong_ordering
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

auto shape_iterator::operator*() const -> game_point
{
  return m_point;
}

auto shape_iterator::operator->() const -> const game_point*
{
  return &m_point;
}

auto shape_iterator::GetCurrentPoint() const -> game_point
{
  auto radiusX = ( m_shapeGenerator->m_rect.bottomRight.x - m_shapeGenerator->m_rect.topLeft.x ) / 2;
  auto radiusY = ( m_shapeGenerator->m_rect.bottomRight.y - m_shapeGenerator->m_rect.topLeft.y ) / 2;

  auto angleInRadians = DEGTORAD(m_currentAngle);
  auto cx = radiusX * sin(angleInRadians);
  auto cy = radiusY * cos(angleInRadians);

  auto x = m_shapeGenerator->m_rect.topLeft.x + radiusX;
  auto y = m_shapeGenerator->m_rect.bottomRight.y + radiusY;

  auto noise = ( psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy)) + 5 ) / 6;
  
  return {x + cx * noise, y + cy * noise};
}

shape_generator::shape_generator(game_rect rect, int sides) : m_rect(rect), m_sides(sides), m_angleIncrement(360 / sides)
{
}

auto shape_generator::begin() const -> shape_iterator
{
  return shape_iterator(this, 0);
}

auto shape_generator::end() const -> shape_iterator
{
  return shape_iterator(this, 360);
}
