#pragma once

class fractional_counter
{

public:

  fractional_counter(float value);

  auto Update(float increment) -> float;
  auto Reset() -> void;
  auto Normalize() -> float;

private:

  float m_value;
  float m_current;
  
};
