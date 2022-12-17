#ifndef _system_timer_
#define _system_timer_

float GetElapsedTimeInSeconds(int64_t ticks, int64_t ticksPerSecond);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t pausedTicks, int64_t ticksPerSecond);

namespace performance_counter
{
  int64_t QueryFrequency();
  int64_t QueryValue();

  struct data
  {
    int64_t frequency;
    int64_t initialValue;
    int64_t currentValue;
  };

};

#endif
