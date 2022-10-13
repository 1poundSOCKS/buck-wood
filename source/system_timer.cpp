#include "system_timer.h"

system_timer::system_timer()
{
  LARGE_INTEGER perfFrequencyTmp;
  LARGE_INTEGER initialTicksTmp;
  QueryPerformanceFrequency(&perfFrequencyTmp);
  ticksPerSecond = perfFrequencyTmp.QuadPart;
  QueryPerformanceCounter(&initialTicksTmp);
  initialTicks = totalTicks = initialTicksTmp.QuadPart;
}

void UpdateSystemTimer(system_timer& timer)
{
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  int64_t totalTicks = ticks.QuadPart;
  timer.intervalTicks = totalTicks - timer.totalTicks;
  timer.totalTicks = totalTicks;
}

float GetRunTimeInSeconds(const system_timer& timer)
{
  return static_cast<float>(timer.totalTicks - timer.initialTicks) / static_cast<float>(timer.ticksPerSecond);
}

float GetIntervalTimeInSeconds(const system_timer& timer)
{
  return static_cast<float>(timer.intervalTicks) / static_cast<float>(timer.ticksPerSecond);
}
