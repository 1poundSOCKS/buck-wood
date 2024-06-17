#pragma once

#include "collision_type.h"
#include "default_object_geometry.h"

class geometry_collision
{

public:

  enum class result { none, collision, containment };

  geometry_collision(collision_type collisionType);
  auto operator()(const default_object_geometry& object1, const default_object_geometry& object2) const -> result;

private:

  static [[nodiscard]] auto CheckDirect2D(const default_object_geometry& object1, const default_object_geometry& object2) noexcept -> result;
  auto CheckBoundaryContainment(RECT_F object1Bounds, RECT_F object2Bounds) const -> bool;

private:

  collision_type m_type;

};
