#include "pch.h"
#include "input_state.h"

void input_state::RefreshKeyboard(IDirectInputDevice8* keyboard)
{
  HRESULT hr = keyboard->GetDeviceState(sizeof(keyboardState), keyboardState.data);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
	}
}

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window)
{
  winrt::com_ptr<IDirectInput8> directInput;
  HRESULT hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDirectInputDevice8> keyboard;
  hr = directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  keyboard->Acquire();

  return keyboard;
}

void ReadKeyboardState(IDirectInputDevice8* keyboard, keyboard_state& state)
{
  HRESULT hr = keyboard->GetDeviceState(sizeof(state.data), state.data);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
	}
}

bool KeyPressed(const screen_input_state& screenInputState, uint8_t keyCode)
{
  return screenInputState.keyboardState.data[keyCode] & 0x80 && !(screenInputState.previousKeyboardState.data[keyCode] & 0x80);
}

float GetRatioMouseX(const screen_input_state& screenInputState)
{
  return screenInputState.renderTargetMouseData.x / screenInputState.renderTargetMouseData.size.width;
}

float GetRatioMouseY(const screen_input_state& screenInputState)
{
  return screenInputState.renderTargetMouseData.y / screenInputState.renderTargetMouseData.size.height;
}

bool MouseMoved(const screen_input_state& screenInputState)
{
  return
    screenInputState.windowData.mouse.x != screenInputState.previousWindowData.mouse.x ||
    screenInputState.windowData.mouse.y != screenInputState.previousWindowData.mouse.y;
}

bool LeftMouseButtonDrag(const screen_input_state& screenInputState)
{
  return
    !screenInputState.windowData.mouse.leftButtonDown &&
    screenInputState.previousWindowData.mouse.leftButtonDown && 
    MouseMoved(screenInputState);
}

bool LeftMouseButtonReleased(const screen_input_state& screenInputState)
{
  return screenInputState.previousWindowData.mouse.leftButtonDown && !screenInputState.windowData.mouse.leftButtonDown;
}
