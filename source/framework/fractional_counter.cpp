#include "pch.h"
#include "fractional_counter.h"

fractional_counter::fractional_counter(float value) : m_value { value }
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
