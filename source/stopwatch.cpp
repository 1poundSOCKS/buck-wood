#include "pch.h"
#include "stopwatch.h"

auto stopwatch::Start(int64_t totalTicks) -> void
{
  m_totalTicks = totalTicks;
}

auto stopwatch::Update(int64_t elapsedTicks) -> int64_t
{
  m_totalTicks -= elapsedTicks;
  return max(0, m_totalTicks);
}
