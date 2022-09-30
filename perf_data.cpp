#include "perf_data.h"

perf_data::perf_data(const LARGE_INTEGER& perfFrequency, const LARGE_INTEGER& initialTicks, const LARGE_INTEGER& ticks, const LARGE_INTEGER& previousTicks)
{
  totalTicks = ticks.QuadPart - initialTicks.QuadPart;
  frameTicks = ticks.QuadPart - previousTicks.QuadPart;
  frameTimeSeconds = static_cast<double>(frameTicks) / static_cast<double>(perfFrequency.QuadPart);
  fps = frameTicks ? perfFrequency.QuadPart / frameTicks : 0;
}
