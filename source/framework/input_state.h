#ifndef _input_state_
#define _input_state_

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <d2d1.h>
#include <dinput.h>
#include <winrt/base.h>

struct input_state
{
  input_state();

  void RefreshKeyboard(const winrt::com_ptr<IDirectInputDevice8>& keyboard);
  void UpdateMouse(LPARAM clientMouseX, LPARAM clientMouseY, const RECT& clientRect, bool leftMouseButtonDown, bool rightMouseButtonDown, const D2D1_SIZE_F& renderTargetSize);

  unsigned char keyboardState[256];
  bool leftMouseButtonDown = false, rightMouseButtonDown = false;
  RECT clientRect;
  LPARAM clientMouseX = 0, clientMouseY = 0;
  float ratioMouseX = 0, ratioMouseY = 0;
  D2D1_SIZE_F renderTargetSize;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
};

#endif
