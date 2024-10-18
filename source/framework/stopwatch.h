#pragma once

#include "performance_counter.h"

class stopwatch
{

public:

  stopwatch(int64_t ticks) : m_ticks { ticks }
  {
  }

  stopwatch(float seconds) : m_ticks { performance_counter::CalculateTicks(seconds) }
  {
  }

  auto Update(int64_t ticks) -> bool
  {
    return ( m_ticks -= ticks ) > 0 ? true : false;
  }

  auto Expired() const -> bool
  {
    return m_ticks > 0 ? false : true;
  }

  auto Reset(float seconds) noexcept -> void
  {
    m_ticks = performance_counter::CalculateTicks(seconds);
  }

private:

  int64_t m_ticks { 0 };

};
