#ifndef _input_state_
#define _input_state_

#include "main_window.h"

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

struct input_state
{
  void RefreshKeyboard(IDirectInputDevice8* keyboard);

  keyboard_state keyboardState;
  bool leftMouseButtonDown = false, rightMouseButtonDown = false;
  client_mouse_data clientMouseData;
  render_target_mouse_data renderTargetMouseData;
};

struct screen_input_state
{
  window_data windowData;
  window_data previousWindowData;
  keyboard_state keyboardState;
  keyboard_state previousKeyboardState;
  render_target_mouse_data renderTargetMouseData;
  render_target_mouse_data previousRenderTargetMouseData;
};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);
void ReadKeyboardState(IDirectInputDevice8* keyboard, keyboard_state& state);
bool KeyPressed(const screen_input_state& screenInputState, uint8_t keyCode);
float GetRatioMouseX(const screen_input_state& screenInputState);
float GetRatioMouseY(const screen_input_state& screenInputState);
bool LeftMouseButtonDrag(const screen_input_state& screenInputState);
bool LeftMouseButtonReleased(const screen_input_state& screenInputState);

#endif
