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

struct stopwatch
{
  stopwatch(const system_timer& systemTimer, float seconds);

  const system_timer& systemTimer;
  float seconds;
  float remainingTimeInSeconds;
  int64_t initialTicks = 0;
  int64_t totalTicks = 0;
  int64_t intervalTicks = 0;
  int64_t pausedTicks = 0;
  bool paused = true;
};

void UpdateTimer(system_timer& timer);
void UpdateTimer(game_timer& timer);
void ResetTimer(game_timer& gameTimer);
void UpdateStopwatch(stopwatch& stopwatch);
void ResetStopwatch(stopwatch& stopwatch, float seconds);

float GetTotalTimeInSeconds(const system_timer& timer);
float GetIntervalTimeInSeconds(const system_timer& timer);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t pausedTicks, int64_t ticksPerSecond);

#endif
