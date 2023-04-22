#include "pch.h"
#include "performance_counter.h"

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

int64_t performance_counter::QueryFrequency()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  return frequency.QuadPart;
}

int64_t performance_counter::QueryValue()
{
  LARGE_INTEGER count;
  QueryPerformanceCounter(&count);
  return count.QuadPart;
}
