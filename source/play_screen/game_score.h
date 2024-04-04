#pragma once

class game_score
{

public:

  enum class value_type { total, power_ups };

  game_score(value_type type) : m_type { type }
  {
  }

  [[nodiscard]] auto Type() const -> value_type
  {
    return m_type;
  }
  
  [[nodiscard]] auto Value() const -> int
  {
    return m_value;
  }

  auto Add(int value) -> int
  {
    return m_value += value;
  }

  auto Set(int value) -> int
  {
    return m_value = value;
  }

private:

  value_type m_type;
  int m_value { 0 };

};
