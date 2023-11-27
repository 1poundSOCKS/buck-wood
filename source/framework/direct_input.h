#pragma once

#include "com_singleton.h"

class direct_input : public com_singleton<IDirectInput8>
{

public:

  static auto create(HINSTANCE instance) -> void;

private:

  direct_input(HINSTANCE instance);

};
