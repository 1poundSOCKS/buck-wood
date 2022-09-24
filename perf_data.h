#include <cstdint>
#include <windows.h>

struct perf_data
{
  perf_data(const LARGE_INTEGER& perfFrequency, const LARGE_INTEGER& initialTicks, const LARGE_INTEGER& ticks, const LARGE_INTEGER& previousTicks);

  int64_t totalTicks;
  int64_t frameTicks;
  int64_t fps;
};
