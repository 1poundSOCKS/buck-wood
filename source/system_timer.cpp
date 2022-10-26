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

stopwatch::stopwatch(const system_timer& systemTimer, float seconds) : systemTimer(systemTimer), seconds(seconds), remainingTimeInSeconds(seconds)
{
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

void ResetTimer(game_timer& gameTimer)
{
  gameTimer.ticksPerSecond = gameTimer.systemTimer.ticksPerSecond;
  gameTimer.initialTicks = gameTimer.systemTimer.totalTicks;
  gameTimer.totalTicks = gameTimer.systemTimer.totalTicks;
  gameTimer.intervalTicks = 0;
  gameTimer.pausedTicks = 0;
  gameTimer.paused = false;
}

void UpdateStopwatch(stopwatch& stopwatch)
{
  stopwatch.intervalTicks = stopwatch.systemTimer.totalTicks - stopwatch.totalTicks;
  stopwatch.totalTicks = stopwatch.systemTimer.totalTicks;

  if( stopwatch.paused )
  {
    stopwatch.pausedTicks += stopwatch.intervalTicks;
  }

  auto elapsedTime = GetElapsedTimeInSeconds(stopwatch.initialTicks, stopwatch.totalTicks, stopwatch.pausedTicks, stopwatch.systemTimer.ticksPerSecond);
  auto remainingTime = stopwatch.seconds - elapsedTime;
  stopwatch.remainingTimeInSeconds = max(0, remainingTime);
}

void ResetStopwatch(stopwatch& stopwatch, float seconds)
{
  stopwatch.initialTicks = stopwatch.systemTimer.totalTicks;
  stopwatch.totalTicks = stopwatch.systemTimer.totalTicks;
  stopwatch.seconds = seconds;
  stopwatch.remainingTimeInSeconds = seconds;
  stopwatch.intervalTicks = 0;
  stopwatch.pausedTicks = 0;
  stopwatch.paused = false;
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

float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t pausedTicks, int64_t ticksPerSecond)
{
  return static_cast<float>(endTicks - startTicks - pausedTicks) / static_cast<float>(ticksPerSecond);
}
