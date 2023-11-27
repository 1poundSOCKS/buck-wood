#pragma once

#include "window_data.h"
#include "render_target_mouse_data.h"
#include "keyboard_reader.h"
#include "gamepad_reader.h"

class user_input
{

public:

  // static auto create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void;
  static auto create() -> void;
  static auto destroy() -> void;

  static auto update() -> void;
  
  // static auto keyboardReader() -> const keyboard_reader&;
  static auto gamepadReader() -> const gamepad_reader&;

private:

  // user_input(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice);
  user_input();
  auto Update() -> void;

private:

  static user_input* m_instance;
  // winrt::com_ptr<IDirectInputDevice8> m_keyboard;
  // keyboard_reader m_keyboardReader;
  gamepad_reader m_gamepadReader;

};

// inline auto user_input::keyboardReader() -> const keyboard_reader&
// {
//   return m_instance->m_keyboardReader;
// }

inline auto user_input::gamepadReader() -> const gamepad_reader&
{
  return m_instance->m_gamepadReader;
}
