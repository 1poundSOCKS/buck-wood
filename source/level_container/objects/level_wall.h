#pragma once

#include "base_object.h"

class level_wall : public base_object
{

public:

  level_wall(POINT_2F position, SCALE_2F scale, float angle) noexcept : base_object { position, scale, angle }
  {
  }

};
