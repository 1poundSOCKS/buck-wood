#pragma once

class health_status
{

public:

  health_status(int maxValue);

  auto ApplyDamage(int value) -> int;

  [[nodiscard]] auto GetDamagePercentage() -> float;

private:

  int m_maxValue { 0 };
  int m_value { 0 };

};

inline health_status::health_status(int maxValue) : m_maxValue { maxValue }
{
}

inline auto health_status::ApplyDamage(int value) -> int
{
  m_value += value;
  m_value = min(m_value, m_maxValue);
  return m_maxValue - m_value;
}

[[nodiscard]] inline auto health_status::GetDamagePercentage() -> float
{
  return static_cast<float>(m_value) / static_cast<float>(m_maxValue) * 100.0f;
}
