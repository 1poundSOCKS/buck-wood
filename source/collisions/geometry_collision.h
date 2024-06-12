#pragma once

#include "collision_type.h"
#include "default_object_geometry.h"

class geometry_collision
{

public:

  geometry_collision(collision_type collsionType);
  auto operator()(default_object_geometry& object1, default_object_geometry& object2) -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(default_object_geometry& object1, default_object_geometry& object2) noexcept -> bool;

private:

  collision_type m_type;

};
