#pragma once

class clock_frequency
{
public:

  static auto get() -> int64_t;

private:
  
  static clock_frequency* m_clockFrequency;
  clock_frequency();
  int64_t m_value;
};
