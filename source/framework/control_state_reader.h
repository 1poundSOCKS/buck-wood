#ifndef _control_state_reader_
#define _control_state_reader_

#include "main_window.h"
#include "input_state.h"

template <typename control_state_type>
struct control_state_reader
{
  ID2D1RenderTarget* renderTarget;
  window_data& windowData;
  IDirectInputDevice8* keyboard;

  void Read(control_state_type& controlState) const
  {
    keyboard_state keyboardState;
    HRESULT hr = keyboard->GetDeviceState(sizeof(keyboardState.data), reinterpret_cast<LPVOID>(keyboardState.data));
    if(FAILED(hr))
    {
      if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
    }

    ReadControlState(renderTarget, windowData, keyboardState, controlState);
  }
};

#endif
