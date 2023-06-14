#include "pch.h"
#include "perf_data.h"

performance::frame_data::frame_data() : m_intervalIndex(-1)
{
  for( auto& interval : m_intervalTimes )
  {
    interval = 0;
  }

  m_frequency = performance_counter::QueryFrequency();
  m_value = m_lastValue = performance_counter::QueryValue();
}

auto performance::frame_data::Update() -> void
{
  m_lastValue = m_value;
  m_value = performance_counter::QueryValue();
  auto intervalTime = m_value - m_lastValue;
  m_intervalIndex = ++m_intervalIndex % m_intervalTimes.size();
  m_intervalTimes[m_intervalIndex] = intervalTime;
}

[[nodiscard]] auto performance::frame_data::GetFPS() const -> int64_t
{
  int64_t sumOfIntervalTimes = std::accumulate(m_intervalTimes.cbegin(), m_intervalTimes.cend(), static_cast<int64_t>(0));
  int64_t averageIntervalTime = sumOfIntervalTimes / m_intervalTimes.size();
  return averageIntervalTime ? m_frequency / averageIntervalTime : 0;
}
