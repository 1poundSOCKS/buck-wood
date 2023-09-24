#pragma once

// #define XINPUT_GAMEPAD_BACK   0x0020
// #define XINPUT_GAMEPAD_A      0x1000

class gamepad_buttons
{

public:

  gamepad_buttons() = default;
  gamepad_buttons(WORD state);

  [[nodiscard]] auto APressed() const -> bool;
  [[nodiscard]] auto BackPressed() const -> bool;
  [[nodiscard]] auto UpPressed() const -> bool;
  [[nodiscard]] auto DownPressed() const -> bool;

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

[[nodiscard]] inline auto gamepad_buttons::BackPressed() const -> bool
{
  return m_state & XINPUT_GAMEPAD_BACK ? true : false;
}

[[nodiscard]] inline auto gamepad_buttons::UpPressed() const -> bool
{
  return m_state & XINPUT_GAMEPAD_DPAD_UP ? true : false;
  
}

[[nodiscard]] inline auto gamepad_buttons::DownPressed() const -> bool
{
  return m_state & XINPUT_GAMEPAD_DPAD_DOWN ? true : false;
}
