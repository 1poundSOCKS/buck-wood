#pragma once

class game_score
{

public:

  [[nodiscard]] auto Value() const -> int
  {
    return m_value;
  }

  auto Add(int value) -> int
  {
    return m_value += value;
  }

private:

  int m_value { 0 };

};
