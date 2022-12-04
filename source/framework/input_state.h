#ifndef _input_state_
#define _input_state_

#include "main_window.h"

struct client_mouse_data
{
  RECT rect;
  float x = 0, y = 0;
};

struct render_target_mouse_data
{
  D2D_SIZE_F size;
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

struct keyboard_state_reader
{
  IDirectInputDevice8* keyboard;
  void Read(keyboard_state& state);
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
};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);

#endif
