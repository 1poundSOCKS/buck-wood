#include "pch.h"
#include "input_state.h"

input_state::input_state()
{
  ZeroMemory(keyboardState, sizeof(keyboardState));
}

void input_state::RefreshKeyboard(IDirectInputDevice8* keyboard)
{
  HRESULT hr = keyboard->GetDeviceState(sizeof(keyboardState), keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
	}
}
