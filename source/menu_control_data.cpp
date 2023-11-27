#include "pch.h"
#include "menu_control_data.h"

menu_control_data::menu_control_data()
{
  const auto& gamepadReader = user_input::gamepadReader();

  const auto& currentGamepadState = gamepadReader.CurrentState();
  const auto& previousGamepadState = gamepadReader.PreviousState();

  gamepad_button_reader gamepadButtonReader { gamepadReader };

  gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY() };
  gamepad_thumbstick previousLeftThumbstick { previousGamepadState.ThumbLX(), previousGamepadState.ThumbLY() };

  auto thumbstickUp = leftThumbstick.UpPressed() && !previousLeftThumbstick.UpPressed();
  m_up = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_UP) || thumbstickUp || keyboard_reader::pressed(DIK_UP);

  auto thumbstickDown = leftThumbstick.DownPressed() && !previousLeftThumbstick.DownPressed();
  m_down = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_DOWN) || thumbstickDown || keyboard_reader::pressed(DIK_DOWN);

  auto thumbstickLeft = leftThumbstick.LeftPressed() && !previousLeftThumbstick.LeftPressed();
  m_left = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_LEFT) || thumbstickLeft || keyboard_reader::pressed(DIK_LEFT);

  auto thumbstickRight = leftThumbstick.RightPressed() && !previousLeftThumbstick.RightPressed();
  m_right = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_DPAD_RIGHT) || thumbstickRight || keyboard_reader::pressed(DIK_RIGHT);

  m_select = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_A) || keyboard_reader::pressed(DIK_SPACE) || keyboard_reader::pressed(DIK_RETURN);

  m_back = gamepadButtonReader.Pressed(XINPUT_GAMEPAD_B) || keyboard_reader::pressed(DIK_ESCAPE);
}
