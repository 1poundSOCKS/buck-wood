#include "pch.h"
#include "perf_data.h"

performance::frame_data::frame_data() : intervalIndex(-1)
{
  for( auto& interval : intervalTimes ) { interval = 0; }
  frequency = performance_counter::QueryFrequency();
  value = lastValue = performance_counter::QueryValue();
}

void performance::UpdateFrameData(performance::frame_data& frameData)
{
  frameData.lastValue = frameData.value;
  frameData.value = performance_counter::QueryValue();
  auto intervalTime = frameData.value - frameData.lastValue;
  frameData.intervalIndex = ++frameData.intervalIndex % frameData.intervalTimes.size();
  frameData.intervalTimes[frameData.intervalIndex] = intervalTime;
}

int64_t performance::GetFPS(const performance::frame_data& frameData)
{
  int64_t sumOfIntervalTimes = std::accumulate(frameData.intervalTimes.cbegin(), frameData.intervalTimes.cend(), static_cast<int64_t>(0));
  int64_t averageIntervalTime = sumOfIntervalTimes / frameData.intervalTimes.size();
  return averageIntervalTime ? frameData.frequency / averageIntervalTime : 0;
}
