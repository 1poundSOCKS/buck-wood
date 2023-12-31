#pragma once

#include "reload_timer.h"

class reload_counter
{

public:

  reload_counter(float reloadTime, int maxCount) : m_timer { reloadTime }, m_maxCount { maxCount }
  {
  }

  auto Update(float interval) -> void
  {
    m_count += ( m_timer.Update(interval) && m_count < m_maxCount ) ? 1 : 0;
  }

  [[nodiscard]] auto Get(int count) -> int
  {
    return std::exchange(m_count, m_count - min(count, m_count));
  }

private:

  reload_timer m_timer;
  int m_maxCount;
  int m_count { 0 };

};
