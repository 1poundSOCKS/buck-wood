#pragma once

#define XINPUT_GAMEPAD_A 0x1000

class gamepad_buttons
{

public:

  gamepad_buttons() = default;
  gamepad_buttons(WORD state);

  [[nodiscard]] auto APressed() const -> bool;

private:

  WORD m_state { 0 };

};

inline gamepad_buttons::gamepad_buttons(WORD state) : m_state { state }
{
}

[[nodiscard]] inline auto gamepad_buttons::APressed() const -> bool
{
  return m_state & XINPUT_GAMEPAD_A ? true : false;
}
