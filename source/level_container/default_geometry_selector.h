#pragma once

#include "level_geometries.h"

class default_geometry_selector
{
public:

  template <typename object_type> [[nodiscard]] auto get(const object_type& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return level_geometries::get(object);
  }
};
