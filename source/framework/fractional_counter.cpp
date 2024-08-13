#include "pch.h"
#include "fractional_counter.h"

fractional_counter::fractional_counter(float value) : m_value { value }, m_current { 0.0f }
{
}

auto fractional_counter::Update(float increment) -> float
{
  m_current += increment;
  return m_current / m_value;
}

auto fractional_counter::Reset() -> void
{
  m_current = 0;
}

auto fractional_counter::Normalize() -> float
{
  // return m_current = std::max(0.0f, m_current - m_value);
  while( m_current >= m_value )
  {
    m_current -= m_value;
  }

  return m_current;
}
