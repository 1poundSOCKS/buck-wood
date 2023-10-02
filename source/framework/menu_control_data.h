#pragma once

#include "screen_input_state.h"
#include "gamepad_buttons.h"
#include "gamepad_thumbstick.h"

class menu_control_data
{

public:

  menu_control_data(const screen_input_state& screenInputState);

  [[nodiscard]] auto GamepadAttached() const -> bool;

  [[nodiscard]] auto Up() const -> bool;
  [[nodiscard]] auto Down() const -> bool;
  [[nodiscard]] auto Select() const -> bool;
  [[nodiscard]] auto Back() const -> bool;

private:

  bool m_gamepadAttached { false };
  bool m_up { false };
  bool m_down { false };
  bool m_select { false };
  bool m_back { false };

};

inline menu_control_data::menu_control_data(const screen_input_state& screenInputState)
{
  gamepad_buttons currentButtonState { screenInputState.gamepadState.Buttons() };
  gamepad_buttons previousButtonState { screenInputState.previousGamepadState.Buttons() };

  gamepad_thumbstick leftThumbstick { screenInputState.gamepadState.ThumbLX(), screenInputState.gamepadState.ThumbLY() };
  gamepad_thumbstick previousLeftThumbstick { screenInputState.previousGamepadState.ThumbLX(), screenInputState.previousGamepadState.ThumbLY() };
  
  auto buttonUp = currentButtonState.UpPressed() && !previousButtonState.UpPressed();
  auto thumbstickUp = leftThumbstick.UpPressed() && !previousLeftThumbstick.UpPressed();
  m_up = buttonUp || thumbstickUp;

  auto buttonDown = currentButtonState.DownPressed() && !previousButtonState.DownPressed();
  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  m_down = buttonDown || thumbstickDown;

  m_select = currentButtonState.APressed() && !previousButtonState.APressed() ? true : false;

  m_back = currentButtonState.BackPressed() && !previousButtonState.BackPressed() ? true : false;
}

[[nodiscard]] inline auto menu_control_data::GamepadAttached() const -> bool
{
  return m_gamepadAttached;
}

[[nodiscard]] inline auto menu_control_data::Up() const -> bool
{
  return m_up;
}

[[nodiscard]] inline auto menu_control_data::Down() const -> bool
{
  return m_down;
}

[[nodiscard ]] inline auto menu_control_data::Select() const -> bool
{
  return m_select;
}

[[nodiscard ]] inline auto menu_control_data::Back() const -> bool
{
  return m_back;
}
