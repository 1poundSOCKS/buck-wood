#pragma once

#include "main_window.h"
#include "gamepad_state.h"

struct client_mouse_data
{
  RECT rect;
  float x = 0, y = 0;
};

class keyboard_state
{
public:

  keyboard_state();

  auto Update(IDirectInputDevice8* keyboard) -> void;
  auto operator=(const keyboard_state& state) -> const keyboard_state&;
  [[nodiscard]] auto Down(int key) const -> bool;

private:

  unsigned char m_data[256];

};

inline keyboard_state::keyboard_state()
{
  ::ZeroMemory(m_data, sizeof(m_data));
}

inline auto keyboard_state::operator=(const keyboard_state& state) -> const keyboard_state&
{
  ::memcpy(m_data, state.m_data, sizeof(m_data));
  return *this;
}

[[nodiscard]] inline auto keyboard_state::Down(int key) const -> bool
{
  return m_data[key] & 0x80 ? true : false;
}

class keyboard_reader
{

public:

  [[nodiscard]] auto CurrentState() const -> const keyboard_state&;
  [[nodiscard]] auto PreviousState() const -> const keyboard_state&;

  auto Update(IDirectInputDevice8* keyboard) -> void;

  [[nodiscard]] auto Pressed(int key) const -> bool;

private:

  keyboard_state m_currentState;
  keyboard_state m_previousState;
  
};

[[nodiscard]] inline auto keyboard_reader::CurrentState() const -> const keyboard_state&
{
  return m_currentState;
}

[[nodiscard]] inline auto keyboard_reader::PreviousState() const -> const keyboard_state&
{
  return m_previousState;
}

[[nodiscard]] inline auto keyboard_reader::Pressed(int key) const -> bool
{
  return m_currentState.Down(key) && !m_previousState.Down(key);
}

struct screen_input_state
{
  window_data windowData;
  window_data previousWindowData;
  keyboard_reader keyboardReader;
  render_target_mouse_data renderTargetMouseData;
  render_target_mouse_data previousRenderTargetMouseData;
  gamepad_state gamepadState;
  gamepad_state previousGamepadState;
};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);
float GetRatioMouseX(const screen_input_state& screenInputState);
float GetRatioMouseY(const screen_input_state& screenInputState);
bool LeftMouseButtonDrag(const screen_input_state& screenInputState);
bool LeftMouseButtonReleased(const screen_input_state& screenInputState);
