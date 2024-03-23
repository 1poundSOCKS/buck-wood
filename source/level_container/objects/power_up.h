#pragma once

#include "framework.h"
#include "base_object.h"

class power_up : public base_object
{

public:

  power_up(POINT_2F position, VELOCITY_2F velocity) : base_object { position , velocity, { 20.0f, 20.0f }, 0 }
  {
  }

};
