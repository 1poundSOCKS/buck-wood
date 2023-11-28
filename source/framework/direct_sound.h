#pragma once

#include "com_singleton.h"

class direct_sound : public com_singleton<IDirectSound8>
{

public:

  static auto create(HWND wnd) -> void;

private:

  direct_sound(HWND wnd);

};
