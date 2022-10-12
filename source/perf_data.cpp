#include "perf_data.h"

perf_data::perf_data()
{
  QueryPerformanceFrequency(&perfFrequency);
  QueryPerformanceCounter(&initialTicks);
  for( int i = 0; i < fpsFrameCount; i++ ) { fpsFrames[i] = 0; }
}

void UpdatePerformanceData(perf_data& perfData)
{
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  
  perfData.totalTicks = ticks.QuadPart - perfData.initialTicks.QuadPart;
  perfData.frameTicks = ticks.QuadPart - perfData.previousTicks.QuadPart;
  perfData.frameTimeSeconds = static_cast<float>(perfData.frameTicks) / static_cast<float>(perfData.perfFrequency.QuadPart);
  float fpsTmp = static_cast<float>(perfData.perfFrequency.QuadPart) / static_cast<float>(perfData.frameTicks);
  perfData.fps = static_cast<int64_t>(fpsTmp + 0.5f);
  perfData.previousTicks = ticks;
  
  perfData.fpsFrames[perfData.fpsFrameIndex] = perfData.fps;
  perfData.fpsFrameIndex = ++perfData.fpsFrameIndex % perfData.fpsFrameCount;
  
  float fpsTotal = 0;
  for( int i = 0; i < perfData.fpsFrameCount; i++ ) { fpsTotal += perfData.fpsFrames[i]; }
  perfData.fpsAverage = ( fpsTotal / perfData.fpsFrameCount + 0.5f );
}
