#ifndef _perf_data_
#define _perf_data_

#include "timers.h"

namespace performance
{
  struct frame_data
  {
    frame_data();

    int64_t frequency;
    int64_t lastValue;
    int64_t value;

    std::array<int64_t, 20> intervalTimes;
    int intervalIndex;
  };

  void UpdateFrameData(frame_data& frameData);
  int64_t GetFPS(const frame_data& frameData);
};

#endif
