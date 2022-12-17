#ifndef _system_timer_
#define _system_timer_

// struct system_timer
// {
//   system_timer();

//   int64_t ticksPerSecond = 0;
//   int64_t initialTicks = 0;
//   int64_t totalTicks = 0;
//   int64_t intervalTicks = 0;
// };

// struct stopwatch
// {
//   stopwatch(const system_timer& systemTimer, int timeNumerator=0, int timeDenominator=1);

//   const system_timer& systemTimer;
//   int timeNumerator = 0;
//   int timeDenominator = 1;
//   int64_t initialTicks = 0;
//   int64_t currentTicks = 0;
//   int64_t endTicks = 0;
//   int64_t pausedTicks = 0;
//   bool paused = true;
// };

// int64_t QueryPerformanceFrequency();
// int64_t QueryPerformanceCounter();

// void UpdateTimer(system_timer& timer);
// void UpdateStopwatch(stopwatch& stopwatch);
// void ResetStopwatch(stopwatch& stopwatch, int timeNumerator, int timeDenominator=1);
// void ResetStopwatch(stopwatch& stopwatch);

// float GetTotalTimeInSeconds(const system_timer& timer);
// float GetIntervalTimeInSeconds(const system_timer& timer);
float GetElapsedTimeInSeconds(int64_t ticks, int64_t ticksPerSecond);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t ticksPerSecond);
float GetElapsedTimeInSeconds(int64_t startTicks, int64_t endTicks, int64_t pausedTicks, int64_t ticksPerSecond);
// int64_t GetTicksRemaining(const stopwatch& stopwatch);
// float GetTimeRemainingInSeconds(const stopwatch& stopwatch);

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
