#ifndef _perf_data_
#define _perf_data_

#include <cstdint>
#include <windows.h>
#include <list>
#include <string>

struct perf_data
{
  perf_data();

  LARGE_INTEGER perfFrequency;
  LARGE_INTEGER initialTicks;
  LARGE_INTEGER previousTicks;
  int64_t totalTicks;
  int64_t frameTicks;
  float frameTimeSeconds;
  int64_t fps;
  float fpsFrames[20];
  const int fpsFrameCount = sizeof(fpsFrames) / sizeof(float);
  int fpsFrameIndex = 0;
  int fpsAverage = 0;
};

void UpdatePerformanceData(perf_data& perfData);

#endif
