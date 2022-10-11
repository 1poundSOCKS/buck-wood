#include "perf_data.h"

perf_data::perf_data(const LARGE_INTEGER& perfFrequency, const LARGE_INTEGER& initialTicks, const LARGE_INTEGER& ticks, const LARGE_INTEGER& previousTicks)
{
  totalTicks = ticks.QuadPart - initialTicks.QuadPart;
  frameTicks = ticks.QuadPart - previousTicks.QuadPart;
  frameTimeSeconds = static_cast<float>(frameTicks) / static_cast<float>(perfFrequency.QuadPart);
  float fpsTmp = static_cast<float>(perfFrequency.QuadPart) / static_cast<float>(frameTicks);
  fps = static_cast<int64_t>(fpsTmp + 0.5f);
}
