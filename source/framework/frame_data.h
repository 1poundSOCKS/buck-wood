#pragma once

class frame_data
{
public:

  frame_data();
  auto Update() -> void;
  [[nodiscard]] auto GetFPS() const -> int64_t;

private:

  int64_t m_frequency;
  int64_t m_lastValue;
  int64_t m_value;

  std::vector<int64_t> m_intervalTimes;
  int m_intervalIndex;
};
