#ifndef _system_timer_
#define _system_timer_

#include <windows.h>
#include <cstdint>

struct system_timer
{
  system_timer();

  int64_t ticksPerSecond = 0;
  int64_t initialTicks = 0;
  int64_t totalTicks = 0;
  int64_t intervalTicks = 0;
};

void UpdateSystemTimer(system_timer& timer);
float GetTotalTimeInSeconds(const system_timer& timer);
float GetIntervalTimeInSeconds(const system_timer& timer);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond);

#endif
