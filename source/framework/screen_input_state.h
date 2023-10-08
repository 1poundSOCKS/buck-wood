#pragma once

#include "main_window.h"
#include "keyboard_reader.h"
#include "gamepad_reader.h"

struct client_mouse_data
{
  RECT rect;
  float x = 0, y = 0;
};

struct screen_input_state
{
  window_data windowData;
  window_data previousWindowData;
  keyboard_reader keyboardReader;
  render_target_mouse_data renderTargetMouseData;
  render_target_mouse_data previousRenderTargetMouseData;
  gamepad_reader gamepadReader;
};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);
float GetRatioMouseX(const screen_input_state& screenInputState);
float GetRatioMouseY(const screen_input_state& screenInputState);
bool LeftMouseButtonDrag(const screen_input_state& screenInputState);
bool LeftMouseButtonReleased(const screen_input_state& screenInputState);
