#pragma once

class stopwatch
{
public:

  auto Start(int64_t totalTicks) -> void;
  auto Update(int64_t elapsedTicks) -> int64_t;

private:

  int64_t m_totalTicks = 0;
};
