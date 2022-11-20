#ifndef _input_state_
#define _input_state_

// #define WIN32_LEAN_AND_MEAN
// #define UNICODE
// #define DIRECTINPUT_VERSION 0x0800

// #include <windows.h>
// #include <d2d1.h>
// #include <dinput.h>
// #include <winrt/base.h>

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

struct input_state
{
  input_state();

  void RefreshKeyboard(const winrt::com_ptr<IDirectInputDevice8>& keyboard);

  unsigned char keyboardState[256];
  bool leftMouseButtonDown = false, rightMouseButtonDown = false;
  client_mouse_data clientMouseData;
  render_target_mouse_data renderTargetMouseData;
};

#endif
