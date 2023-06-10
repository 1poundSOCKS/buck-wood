#include "pch.h"
#include "color_scale.h"

const_color_scale_iterator::const_color_scale_iterator(const color_scale* colorScale, type t) : m_colorScale { colorScale}, m_type { t }
{
}

auto const_color_scale_iterator::operator++() -> const_color_scale_iterator&
{
  if( ++m_currentStep >= m_colorScale->m_steps )
  {
    m_type = type::end;
  }
  else
  {
    UpdateCurrentColor();
  }

  return *this;
}

auto const_color_scale_iterator::operator++(int) -> const_color_scale_iterator
{
  auto tmp = *this;

  if( ++m_currentStep >= m_colorScale->m_steps )
  {
    m_type = type::end;
  }
  else
  {
    UpdateCurrentColor();
  }

  return tmp;
}

auto const_color_scale_iterator::UpdateCurrentColor() -> void
{
  auto cr = m_colorScale->m_end.r - m_colorScale->m_start.r;
  auto cg = m_colorScale->m_end.g - m_colorScale->m_start.g;
  auto cb = m_colorScale->m_end.b - m_colorScale->m_start.b;
  auto ca = m_colorScale->m_end.a - m_colorScale->m_start.a;

  m_currentValue.r = cr * m_currentStep / m_colorScale->m_steps + m_colorScale->m_start.r;
  m_currentValue.g = cg * m_currentStep / m_colorScale->m_steps + m_colorScale->m_start.g;
  m_currentValue.b = cb * m_currentStep / m_colorScale->m_steps + m_colorScale->m_start.b;
  m_currentValue.a = ca * m_currentStep / m_colorScale->m_steps + m_colorScale->m_start.a;
}

auto const_color_scale_iterator::operator*() const -> const D2D_COLOR_F&
{
  return m_currentValue;
}

auto const_color_scale_iterator::operator==(const const_color_scale_iterator& i) const -> bool
{
  if( m_type == type::end && i.m_type == type::end )
  {
    return true;
  }
  else if( m_type == type::end || i.m_type == type::end )
  {
    return false;
  }
  else
  {
    return m_currentStep == i.m_currentStep;
  }
}

color_scale::color_scale(const D2D_COLOR_F& start, const D2D_COLOR_F& end, size_t steps) : m_start { start }, m_end { end }, m_steps { steps }
{
}

[[nodiscard]] auto color_scale::size() const -> size_t
{
  return m_steps;
}

[[nodiscard]] auto color_scale::begin() const -> const_color_scale_iterator
{
  return { this, const_color_scale_iterator::type::begin };
}

[[nodiscard]] auto color_scale::end() const -> const_color_scale_iterator
{
  return { this, const_color_scale_iterator::type::end };
}
