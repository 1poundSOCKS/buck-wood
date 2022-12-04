#include "pch.h"
#include "input_state.h"

// void keyboard_state_reader::Read(keyboard_state& state)
// {
//   HRESULT hr = keyboard->GetDeviceState(sizeof(state.data), state.data);
// 	if(FAILED(hr))
// 	{
// 		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
// 	}
// }

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
