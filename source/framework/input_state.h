#ifndef _input_state_
#define _input_state_

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>
#include <winrt/base.h>

struct input_state
{
  input_state();

  unsigned char keyboardState[256];
  LPARAM mouseX = 0, mouseY = 0;
  bool leftMouseButtonDown = false, rightMouseButtonDown = false;
};

void RefreshKeyboardState(input_state& inputState, const winrt::com_ptr<IDirectInputDevice8>& keyboard);

#endif
