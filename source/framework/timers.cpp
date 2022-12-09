#include "pch.h"
#include "timers.h"

int64_t QueryPerformanceFrequency()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  return frequency.QuadPart;
}

int64_t QueryPerformanceCounter()
{
  LARGE_INTEGER count;
  QueryPerformanceCounter(&count);
  return count.QuadPart;
}

system_timer::system_timer()
{
  LARGE_INTEGER perfFrequencyTmp;
  LARGE_INTEGER initialTicksTmp;
  QueryPerformanceFrequency(&perfFrequencyTmp);
  ticksPerSecond = perfFrequencyTmp.QuadPart;
  QueryPerformanceCounter(&initialTicksTmp);
  initialTicks = totalTicks = initialTicksTmp.QuadPart;
}

stopwatch::stopwatch(const system_timer& systemTimer, int timeNumerator, int timeDenominator) 
: systemTimer(systemTimer), initialTicks(systemTimer.totalTicks), currentTicks(systemTimer.totalTicks), timeNumerator(timeNumerator), timeDenominator(timeDenominator)
{
  endTicks = systemTimer.totalTicks + ( systemTimer.ticksPerSecond * timeNumerator ) / timeDenominator;
}

void UpdateTimer(system_timer& timer)
{
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  int64_t totalTicks = ticks.QuadPart;
  timer.intervalTicks = totalTicks - timer.totalTicks;
  timer.totalTicks = totalTicks;
}

void UpdateStopwatch(stopwatch& stopwatch)
{
  int64_t intervalTicks = stopwatch.systemTimer.totalTicks - stopwatch.currentTicks;

  if( stopwatch.paused )
  {
    stopwatch.pausedTicks += intervalTicks;
    stopwatch.endTicks += intervalTicks;
  }

  stopwatch.currentTicks = min(stopwatch.systemTimer.totalTicks, stopwatch.endTicks);
}

void ResetStopwatch(stopwatch& stopwatch, int timeNumerator, int timeDenominator)
{
  stopwatch.initialTicks = stopwatch.systemTimer.totalTicks;
  stopwatch.currentTicks = stopwatch.systemTimer.totalTicks;
  stopwatch.endTicks = stopwatch.systemTimer.totalTicks + ( stopwatch.systemTimer.ticksPerSecond * timeNumerator ) / timeDenominator;
  stopwatch.pausedTicks = 0;
  stopwatch.paused = true;
}

void ResetStopwatch(stopwatch& stopwatch)
{
  ResetStopwatch(stopwatch, stopwatch.timeNumerator, stopwatch.timeDenominator);
}

float GetTotalTimeInSeconds(const system_timer& timer)
{
  return GetElapsedTimeInSeconds(timer.initialTicks, timer.totalTicks, timer.ticksPerSecond);
}

float GetIntervalTimeInSeconds(const system_timer& timer)
{
  return GetElapsedTimeInSeconds(0, timer.intervalTicks, timer.ticksPerSecond);
}

float GetElapsedTimeInSeconds(int64_t ticks, int64_t ticksPerSecond)
{
  return static_cast<float>(ticks) / static_cast<float>(ticksPerSecond);
}

float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond)
{
  return static_cast<float>(endTicks - startTicks) / static_cast<float>(ticksPerSecond);
}

float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t pausedTicks, int64_t ticksPerSecond)
{
  return static_cast<float>(endTicks - startTicks - pausedTicks) / static_cast<float>(ticksPerSecond);
}

int64_t GetTicksRemaining(const stopwatch& stopwatch)
{
  return stopwatch.endTicks - stopwatch.currentTicks;
}

float GetTimeRemainingInSeconds(const stopwatch& stopwatch)
{
  return static_cast<float>( stopwatch.endTicks - stopwatch.currentTicks ) / stopwatch.systemTimer.ticksPerSecond;
}

int64_t performance_counter::QueryFrequency()
{
  return QueryPerformanceFrequency();
}

int64_t performance_counter::QueryValue()
{
  return QueryPerformanceCounter();
}
