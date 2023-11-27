#pragma once

#include "window_data.h"
#include "render_target_mouse_data.h"
#include "keyboard_reader.h"
#include "gamepad_reader.h"

class user_input
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static auto update() -> void;
  
  static auto gamepadReader() -> const gamepad_reader&;

private:

  user_input();
  auto Update() -> void;

private:

  static user_input* m_instance;
  gamepad_reader m_gamepadReader;

};

inline auto user_input::gamepadReader() -> const gamepad_reader&
{
  return m_instance->m_gamepadReader;
}
