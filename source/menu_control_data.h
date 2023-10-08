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
  const auto& currentGamepadState = screenInputState.gamepadReader.CurrentState();
  const auto& previousGamepadState = screenInputState.gamepadReader.PreviousState();

  gamepad_buttons currentButtonState { currentGamepadState.Buttons() };
  gamepad_buttons previousButtonState { previousGamepadState.Buttons() };

  gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY() };
  gamepad_thumbstick previousLeftThumbstick { previousGamepadState.ThumbLX(), previousGamepadState.ThumbLY() };
  
  auto buttonUp = currentButtonState.UpPressed() && !previousButtonState.UpPressed();
  auto thumbstickUp = leftThumbstick.UpPressed() && !previousLeftThumbstick.UpPressed();
  auto arrowUp = screenInputState.keyboardReader.Pressed(DIK_UP);
  m_up = buttonUp || thumbstickUp | arrowUp;

  auto buttonDown = currentButtonState.DownPressed() && !previousButtonState.DownPressed();
  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  auto arrowDown = screenInputState.keyboardReader.Pressed(DIK_DOWN);
  m_down = buttonDown || thumbstickDown || arrowDown;

  auto spacePressed = screenInputState.keyboardReader.Pressed(DIK_SPACE);
  auto enterPressed = screenInputState.keyboardReader.Pressed(DIK_RETURN);
  auto gamepadAPressed = currentButtonState.APressed() && !previousButtonState.APressed();

  m_select = spacePressed || enterPressed || gamepadAPressed;

  auto escapePressed = screenInputState.keyboardReader.Pressed(DIK_ESCAPE);
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
