#include "perf_data.h"

perf_data::perf_data()
{
  for( int i = 0; i < fpsFrameCount; i++ ) { fpsFrames[i] = 0; }
  timer = std::make_unique<system_timer>();
}

void UpdatePerformanceData(perf_data& perfData)
{
  UpdateSystemTimer(*perfData.timer);

  perfData.fps = static_cast<float>(perfData.timer->ticksPerSecond) / static_cast<float>(perfData.timer->intervalTicks);
  
  perfData.fpsFrames[perfData.fpsFrameIndex] = perfData.fps;
  perfData.fpsFrameIndex = ++perfData.fpsFrameIndex % perfData.fpsFrameCount;
  
  float fpsTotal = 0;
  for( int i = 0; i < perfData.fpsFrameCount; i++ ) { fpsTotal += perfData.fpsFrames[i]; }
  perfData.fpsAverage = ( fpsTotal / perfData.fpsFrameCount + 0.5f );
}
