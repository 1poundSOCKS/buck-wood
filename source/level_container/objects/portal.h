#pragma once

#include "framework.h"
#include "base_object.h"

class portal : public base_object
{

public:

  portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position, scale, angle }
  {
  }

};
