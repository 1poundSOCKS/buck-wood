#pragma once

#include "com_singleton.h"

class d2d_factory : public com_singleton<ID2D1Factory>
{

public:

  static auto create() -> void;

private:

  d2d_factory();

};
