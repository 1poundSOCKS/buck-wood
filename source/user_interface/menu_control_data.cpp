#include "pch.h"
#include "menu_control_data.h"

menu_control_data::menu_control_data()
{
  m_up = gamepad_reader::up_pressed() || keyboard_reader::pressed(DIK_UP);
  m_down = gamepad_reader::down_pressed() || keyboard_reader::pressed(DIK_DOWN);
  m_left = gamepad_reader::left_pressed() || keyboard_reader::pressed(DIK_LEFT);
  m_right = gamepad_reader::right_pressed() || keyboard_reader::pressed(DIK_RIGHT);
  m_select = gamepad_reader::button_pressed(XINPUT_GAMEPAD_A) || keyboard_reader::pressed(DIK_SPACE) || keyboard_reader::pressed(DIK_RETURN);
  m_back = gamepad_reader::button_pressed(XINPUT_GAMEPAD_B) || keyboard_reader::pressed(DIK_ESCAPE);
}
