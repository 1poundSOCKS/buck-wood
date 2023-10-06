#pragma once

#include "main_window.h"
#include "gamepad_state.h"

struct client_mouse_data
{
  RECT rect;
  float x = 0, y = 0;
};

struct keyboard_state
{
  keyboard_state()
  {
    ::ZeroMemory(data, sizeof(data));
  }

  unsigned char data[256];
};

class keyboard_reader
{

public:

  [[nodiscard]] auto CurrentState() const -> const keyboard_state&;
  [[nodiscard]] auto PreviousState() const -> const keyboard_state&;

  auto Update(IDirectInputDevice8* keyboard) -> void;

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

struct screen_input_state
{
  window_data windowData;
  window_data previousWindowData;
  keyboard_state keyboardState;
  keyboard_state previousKeyboardState;
  keyboard_reader keyboardReader;
  render_target_mouse_data renderTargetMouseData;
  render_target_mouse_data previousRenderTargetMouseData;
  gamepad_state gamepadState;
  gamepad_state previousGamepadState;
};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);
void ReadKeyboardState(IDirectInputDevice8* keyboard, keyboard_state& state);
bool KeyPressed(const screen_input_state& screenInputState, uint8_t keyCode);
float GetRatioMouseX(const screen_input_state& screenInputState);
float GetRatioMouseY(const screen_input_state& screenInputState);
bool LeftMouseButtonDrag(const screen_input_state& screenInputState);
bool LeftMouseButtonReleased(const screen_input_state& screenInputState);
