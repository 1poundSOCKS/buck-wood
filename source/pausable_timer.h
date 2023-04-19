#pragma once

class pausable_timer
{
public:

  pausable_timer();
  auto Start(int64_t currentClockTicks) -> void;
  auto Pause() -> void;
  auto Stop() -> void;
  auto Unpause() -> void;
  auto Update(int64_t currentClockTicks) -> int64_t;

private:

  int64_t m_startTicks = 0;
  int64_t m_previousTicks = 0;
  int64_t m_currentTicks = 0;
  int64_t m_pausedTicks = 0;
  bool m_paused = false;
  bool m_stopped = false;
};
