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
  auto radiusX = m_shapeGenerator->m_width / 2;
  auto radiusY = m_shapeGenerator->m_height / 2;

  auto angleInRadians = DEGTORAD(m_currentAngle);

  auto cx = radiusX * sin(angleInRadians);
  auto cy = radiusY * cos(angleInRadians);

  return { m_shapeGenerator->m_x + cx, m_shapeGenerator->m_y - cy };
}

shape_generator::shape_generator(float x, float y, float width, float height, int sides) : 
  m_x { x }, m_y { y }, m_width { width }, m_height { height }, m_sides(sides), m_angleIncrement(360 / sides)
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
