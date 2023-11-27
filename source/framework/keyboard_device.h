#pragma once

#include "com_singleton.h"

class keyboard_device : public com_singleton<IDirectInputDevice8>
{

public:

  static auto create(HINSTANCE instance, HWND window) -> void;

private:

  keyboard_device(HINSTANCE instance, HWND window);

};
