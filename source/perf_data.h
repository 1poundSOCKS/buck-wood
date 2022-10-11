#ifndef _perf_data_
#define _perf_data_

#include <cstdint>
#include <windows.h>
#include <list>
#include <string>

struct perf_data
{
  perf_data(const LARGE_INTEGER& perfFrequency, const LARGE_INTEGER& initialTicks, const LARGE_INTEGER& ticks, const LARGE_INTEGER& previousTicks);

  int64_t totalTicks;
  int64_t frameTicks;
  float frameTimeSeconds;
  int64_t fps;
  std::list<std::wstring> additionalInfo;
};

#endif
