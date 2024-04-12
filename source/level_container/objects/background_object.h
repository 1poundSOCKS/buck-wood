#pragma once

#include "base_object.h"

class background_object : public base_object
{

public:

  background_object(POINT_2F position) : base_object { position, { 5, 5, }, 0 }
  {
  }

};
