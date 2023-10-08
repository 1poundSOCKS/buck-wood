#pragma once

#include "screen_input_state.h"
#include "gamepad_thumbstick.h"
#include "gamepad_button_reader.h"

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

  gamepad_button_reader gamepadButtonReader { screenInputState.gamepadReader };

  gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY() };
  gamepad_thumbstick previousLeftThumbstick { previousGamepadState.ThumbLX(), previousGamepadState.ThumbLY() };
  
  auto thumbstickUp = leftThumbstick.UpPressed() && !previousLeftThumbstick.UpPressed();
  m_up = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_UP) || thumbstickUp || screenInputState.keyboardReader.Pressed(DIK_UP);

  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  m_down = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_DOWN) || thumbstickDown || screenInputState.keyboardReader.Pressed(DIK_DOWN);

  m_select = screenInputState.keyboardReader.Pressed(DIK_SPACE) || screenInputState.keyboardReader.Pressed(DIK_RETURN) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_A);

  m_back = screenInputState.keyboardReader.Pressed(DIK_ESCAPE) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_BACK);
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
