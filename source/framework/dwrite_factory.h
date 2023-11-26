#pragma once

#include "com_singleton.h"

class dwrite_factory : public com_singleton<IDWriteFactory>
{
public:

  static auto create() -> void;

private:
  
  dwrite_factory();

};
