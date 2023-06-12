#include "pch.h"
#include "shape_generator.h"

const_shape_iterator::const_shape_iterator(const shape_generator* shapeGenerator, int initialAngle) : 
  m_shapeGenerator(shapeGenerator), m_currentAngle(initialAngle)
{
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
}

auto const_shape_iterator::operator++() -> const_shape_iterator&
{
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return *this;
}

auto const_shape_iterator::operator++(int) -> const_shape_iterator
{
  const_shape_iterator tmp = *this;
  m_currentAngle += m_shapeGenerator->m_angleIncrement;
  m_currentAngle = min(m_currentAngle, 360);
  if( m_currentAngle < 360 ) m_point = GetCurrentPoint();
  return tmp;
}

auto const_shape_iterator::operator==(const const_shape_iterator& i) const -> bool
{
  return m_currentAngle == i.m_currentAngle;
}

auto const_shape_iterator::operator*() const -> const game_point&
{
  return m_point;
}

auto const_shape_iterator::GetCurrentPoint() const -> game_point
{
  auto centrePoint = m_shapeGenerator->m_rect.CentrePoint();

  auto radiusX = centrePoint.x - m_shapeGenerator->m_rect.topLeft.x;
  auto radiusY = centrePoint.y - m_shapeGenerator->m_rect.topLeft.y;

  auto angleInRadians = DEGTORAD(m_currentAngle);

  auto cx = radiusX * sin(angleInRadians);
  auto cy = radiusY * cos(angleInRadians);

  auto x = centrePoint.x + cx;
  auto y = centrePoint.y - cy;

  return { x, y };
}

shape_generator::shape_generator(game_rect rect, int sides) : m_rect(rect), m_sides(sides), m_angleIncrement(360 / sides)
{
}

auto shape_generator::begin() const -> const_shape_iterator
{
  return const_shape_iterator(this, 0);
}

auto shape_generator::end() const -> const_shape_iterator
{
  return const_shape_iterator(this, 360);
}
