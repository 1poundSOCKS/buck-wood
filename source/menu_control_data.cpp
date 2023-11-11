#include "pch.h"
#include "menu_control_data.h"

menu_control_data::menu_control_data(const screen_input_state& screenInputState)
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

  auto thumbstickLeft = leftThumbstick.LeftPressed() && !previousLeftThumbstick.LeftPressed();
  m_left = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_LEFT) || thumbstickLeft || screenInputState.keyboardReader.Pressed(DIK_LEFT);

  auto thumbstickRight = leftThumbstick.RightPressed() && !previousLeftThumbstick.RightPressed();
  m_right = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_RIGHT) || thumbstickRight || screenInputState.keyboardReader.Pressed(DIK_RIGHT);

  m_select = screenInputState.keyboardReader.Pressed(DIK_SPACE) || screenInputState.keyboardReader.Pressed(DIK_RETURN) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_A);

  m_back = screenInputState.keyboardReader.Pressed(DIK_ESCAPE) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_B);
}