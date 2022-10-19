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

system_timer::system_timer(int fps) : fps(fps)
{
  ticksPerSecond = fps;
  initialTicks = 0;
}

void UpdateSystemTimer(system_timer& timer)
{
  if( timer.fps )
  {
    timer.intervalTicks = 1;
    timer.totalTicks++;
    return;
  }

  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  int64_t totalTicks = ticks.QuadPart;
  timer.intervalTicks = totalTicks - timer.totalTicks;
  timer.totalTicks = totalTicks;
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
