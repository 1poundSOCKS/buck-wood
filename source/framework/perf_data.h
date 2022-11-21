#ifndef _perf_data_
#define _perf_data_

#include "timers.h"

struct perf_data
{
  perf_data();

  float fps;
  float fpsFrames[20];
  const int fpsFrameCount = sizeof(fpsFrames) / sizeof(float);
  int fpsFrameIndex = 0;
  int fpsAverage = 0;
  std::unique_ptr<system_timer> timer;
  int64_t updateScreenStateTicks = 0;
  int64_t renderFrameTicks = 0;
};

void UpdatePerformanceData(perf_data& perfData);

#endif
