#pragma once

class reload_timer
{

public:

  reload_timer() = default;
  reload_timer(int64_t reloadTicks);
  auto Update(int64_t ticks) -> bool;

private:

  int64_t m_reloadTicks { 0 };
  int64_t m_currentTicks { 0 };
};
