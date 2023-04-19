#include "pch.h"
#include "clock_frequency.h"
#include "timers.h"

clock_frequency* clock_frequency::m_clockFrequency = new clock_frequency();

auto clock_frequency::get() -> int64_t
{
  return m_clockFrequency->m_value;
}

clock_frequency::clock_frequency()
{
  m_value = performance_counter::QueryFrequency();
}
