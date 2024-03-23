#pragma once

#include "framework.h"
#include "base_object.h"

class portal : public base_object
{

public:

  portal(POINT_2F position) : base_object { position, { 0, 0 }, { 150.0f, 150.0f }, 0 }
  {
  }

};
