#pragma once

class pausable_timer
{
public:

  pausable_timer();
  auto Start() -> void;
  auto Update() -> int64_t;
  auto Pause() -> void;
  auto Unpause() -> void;
  auto Stop() -> void;

private:

  int64_t m_startTicks = 0;
  int64_t m_previousTicks = 0;
  int64_t m_currentTicks = 0;
  int64_t m_pausedTicks = 0;
  bool m_paused = false;
  bool m_stopped = false;
};
