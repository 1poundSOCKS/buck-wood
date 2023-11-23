#pragma once

#include "window_data.h"
#include "render_target_mouse_data.h"
#include "keyboard_reader.h"
#include "gamepad_reader.h"

class screen_input_state
{

public:

  static auto create(HINSTANCE instance, HWND wnd) -> void;
  static auto destroy() -> void;

  static auto update() -> void;
  
  static auto keyboardReader() -> const keyboard_reader&;
  static auto gamepadReader() -> const gamepad_reader&;

private:

  screen_input_state(HINSTANCE instance, HWND window);
  auto Update() -> void;
  static auto CreateKeyboard(HINSTANCE instance, HWND window) -> winrt::com_ptr<IDirectInputDevice8>;

private:

  static screen_input_state* m_instance;
  winrt::com_ptr<IDirectInputDevice8> m_keyboard;
  keyboard_reader m_keyboardReader;
  gamepad_reader m_gamepadReader;

};

inline auto screen_input_state::update() -> void
{
  m_instance->Update();
}

inline auto screen_input_state::keyboardReader() -> const keyboard_reader&
{
  return m_instance->m_keyboardReader;
}

inline auto screen_input_state::gamepadReader() -> const gamepad_reader&
{
  return m_instance->m_gamepadReader;
}
