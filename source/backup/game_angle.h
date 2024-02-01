#pragma once

class game_angle
{
public:

  game_angle() = default;

  game_angle(float value) : m_value { value }
  {
  }

  operator float() const
  {
    return m_value;
  }

  auto operator+=(float value) -> float
  {
    m_value += value;
    
    while( m_value < 0 )
    {
      m_value += 360;
    }
    
    while( m_value >= 360 )
    {
      m_value -= 360;
    }

    return m_value;
  }

private:

  float m_value { 0 };

};
