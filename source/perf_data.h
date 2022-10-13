#ifndef _perf_data_
#define _perf_data_

#include <cstdint>
#include <windows.h>
#include <list>
#include <string>
#include "system_timer.h"

struct perf_data
{
  perf_data();

  float fps;
  float fpsFrames[20];
  const int fpsFrameCount = sizeof(fpsFrames) / sizeof(float);
  int fpsFrameIndex = 0;
  int fpsAverage = 0;
};

void UpdatePerformanceData(perf_data& perfData, const system_timer& systemTimer);

#endif
