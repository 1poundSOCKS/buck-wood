#pragma once

class gamepad_trigger
{

public:
  
  gamepad_trigger(BYTE value);

  [[nodiscard]] auto Value() const -> BYTE;
  [[nodiscard]] auto Ratio() const -> float;

private:

  BYTE m_value;

};

inline gamepad_trigger::gamepad_trigger(BYTE value) : m_value { value }
{
}

[[nodiscard]] inline auto gamepad_trigger::Value() const -> BYTE
{
  return m_value;
}

[[nodiscard]] inline auto gamepad_trigger::Ratio() const -> float
{
  return static_cast<float>(m_value) / static_cast<float>(UCHAR_MAX);
}
