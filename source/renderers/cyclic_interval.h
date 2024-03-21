#pragma once

class cyclic_interval
{

public:

  constexpr cyclic_interval(float length) : m_length { length }
  {
  }

  constexpr auto get(float interval) const -> float
  {
    float compressedInterval = interval / m_length;
    return compressedInterval - static_cast<int>(compressedInterval);
  }

private:

  float m_length;

};
