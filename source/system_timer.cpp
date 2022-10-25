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
 
game_timer::game_timer(const system_timer& systemTimer) : systemTimer(systemTimer)
{
  ticksPerSecond = systemTimer.ticksPerSecond;
  initialTicks = systemTimer.totalTicks;
  totalTicks = systemTimer.totalTicks;
}

void UpdateTimer(system_timer& timer)
{
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  int64_t totalTicks = ticks.QuadPart;
  timer.intervalTicks = totalTicks - timer.totalTicks;
  timer.totalTicks = totalTicks;
}

void UpdateTimer(game_timer& timer)
{
  timer.intervalTicks = timer.systemTimer.totalTicks - timer.totalTicks;
  timer.totalTicks = timer.systemTimer.totalTicks;

  if( timer.paused )
  {
    timer.pausedTicks += timer.intervalTicks;
  }
}

float GetTotalTimeInSeconds(const system_timer& timer)
{
  return GetElapsedTimeInSeconds(timer.initialTicks, timer.totalTicks, timer.ticksPerSecond);
}

float GetIntervalTimeInSeconds(const system_timer& timer)
{
  return GetElapsedTimeInSeconds(0, timer.intervalTicks, timer.ticksPerSecond);
}

float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond)
{
  return static_cast<float>(endTicks - startTicks) / static_cast<float>(ticksPerSecond);
}
