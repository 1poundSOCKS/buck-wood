#pragma once

#include "com_singleton.h"

class keyboard_device : public com_singleton<IDirectInputDevice8>
{

public:

  static auto create(IDirectInput8* directInput, HWND wnd) -> void;

private:

  keyboard_device(IDirectInput8* directInput, HWND wnd);

};
