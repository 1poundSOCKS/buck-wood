#pragma once

#include "level_geometries.h"

class level_geometry_scale
{

public:

  static [[nodiscard]] auto get(auto&& object) -> SCALE_2F
  {
    return { 1.5f, 1.5f };
  }

};
