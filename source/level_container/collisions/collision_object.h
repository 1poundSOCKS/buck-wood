#pragma once

#include "objects/default_object.h"
#include "level_object_geometry.h"

class collision_object
{

public:

  collision_object(default_object& object) : m_object { object }, m_geometry { object }
  {
  }

private:

  default_object& m_object;
  level_object_geometry m_geometry;

};
