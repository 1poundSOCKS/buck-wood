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
  auto arrowUp = screenInputState.keyboardState.data[DIK_UP] & 0x80 && !(screenInputState.previousKeyboardState.data[DIK_UP] & 0x80) ? true : false;
  m_up = buttonUp || thumbstickUp | arrowUp;

  auto buttonDown = currentButtonState.DownPressed() && !previousButtonState.DownPressed();
  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  auto arrowDown = screenInputState.keyboardState.data[DIK_DOWN] & 0x80 && !(screenInputState.previousKeyboardState.data[DIK_DOWN] & 0x80) ? true : false;
  m_down = buttonDown || thumbstickDown || arrowDown;

  auto spacePressed = screenInputState.keyboardState.data[DIK_SPACE] & 0x80 && !(screenInputState.previousKeyboardState.data[DIK_SPACE] & 0x80) ? true : false;
  auto enterPressed = screenInputState.keyboardState.data[DIK_RETURN] & 0x80 && !(screenInputState.previousKeyboardState.data[DIK_RETURN] & 0x80) ? true : false;
  auto gamepadAPressed = currentButtonState.APressed() && !previousButtonState.APressed();

  m_select = spacePressed || enterPressed || gamepadAPressed;

  auto escapePressed = screenInputState.keyboardState.data[DIK_ESCAPE] & 0x80 && !(screenInputState.previousKeyboardState.data[DIK_ESCAPE] & 0x80) ? true : false;
  auto backPressed = currentButtonState.BackPressed() && !previousButtonState.BackPressed();

  m_back = escapePressed || backPressed;
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
