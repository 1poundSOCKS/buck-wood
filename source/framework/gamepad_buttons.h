#pragma once

class gamepad_buttons
{

public:

  gamepad_buttons() = default;
  gamepad_buttons(WORD state);

  [[nodiscard]] auto Down(int button) const -> bool;

private:

  WORD m_state { 0 };

};

inline gamepad_buttons::gamepad_buttons(WORD state) : m_state { state }
{
}


[[nodiscard]] inline auto gamepad_buttons::Down(int button) const -> bool
{
  return m_state & button ? true : false;
}
