#include "pch.h"
#include "menu_control_data.h"

menu_control_data::menu_control_data()
{
  const auto& currentGamepadState = screen_input_state::gamepadReader().CurrentState();
  const auto& previousGamepadState = screen_input_state::gamepadReader().PreviousState();

  gamepad_button_reader gamepadButtonReader { screen_input_state::gamepadReader() };

  gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY() };
  gamepad_thumbstick previousLeftThumbstick { previousGamepadState.ThumbLX(), previousGamepadState.ThumbLY() };

  const auto& keyboardReader = screen_input_state::keyboardReader();
  
  auto thumbstickUp = leftThumbstick.UpPressed() && !previousLeftThumbstick.UpPressed();
  m_up = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_UP) || thumbstickUp || keyboardReader.Pressed(DIK_UP);

  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  m_down = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_DOWN) || thumbstickDown || keyboardReader.Pressed(DIK_DOWN);

  auto thumbstickLeft = leftThumbstick.LeftPressed() && !previousLeftThumbstick.LeftPressed();
  m_left = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_LEFT) || thumbstickLeft || keyboardReader.Pressed(DIK_LEFT);

  auto thumbstickRight = leftThumbstick.RightPressed() && !previousLeftThumbstick.RightPressed();
  m_right = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_RIGHT) || thumbstickRight || keyboardReader.Pressed(DIK_RIGHT);

  m_select = keyboardReader.Pressed(DIK_SPACE) || keyboardReader.Pressed(DIK_RETURN) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_A);

  m_back = keyboardReader.Pressed(DIK_ESCAPE) || gamepadButtonReader.Pressed(XINPUT_GAMEPAD_B);
}
