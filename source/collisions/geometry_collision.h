#pragma once

#include "collision_type.h"
#include "default_object_geometry.h"

class geometry_collision
{

public:

  geometry_collision(collision_type collsionType);
  auto operator()(const default_object_geometry& object1, const default_object_geometry& object2) const -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(const default_object_geometry& object1, const default_object_geometry& object2) noexcept -> bool;

private:

  collision_type m_type;

};
