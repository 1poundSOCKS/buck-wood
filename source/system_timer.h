#ifndef _system_timer_
#define _system_timer_

#include <windows.h>
#include <cstdint>
#include <memory>

struct system_timer
{
  system_timer();

  int64_t ticksPerSecond = 0;
  int64_t initialTicks = 0;
  int64_t totalTicks = 0;
  int64_t intervalTicks = 0;
};

using system_timer_ptr = std::unique_ptr<system_timer>;

struct game_timer
{
  game_timer(const system_timer& systemTimer);

  const system_timer& systemTimer;
  int64_t ticksPerSecond = 0;
  int64_t initialTicks = 0;
  int64_t totalTicks = 0;
  int64_t intervalTicks = 0;
  int64_t pausedTicks = 0;
  bool paused = true;
};

using game_timer_ptr = std::unique_ptr<game_timer>;

void UpdateTimer(system_timer& timer);
void UpdateTimer(game_timer& timer);

float GetTotalTimeInSeconds(const system_timer& timer);
float GetIntervalTimeInSeconds(const system_timer& timer);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond);

#endif
