#pragma once

class fractional_counter
{

public:

  fractional_counter(float value);

  auto Update(float increment) -> float;
  auto Reset() -> void;

private:

  float m_value;
  float m_current;
  
};
