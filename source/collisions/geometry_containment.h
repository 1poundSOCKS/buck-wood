#pragma once

#include "collision_type.h"
#include "default_object_geometry.h"

class geometry_containment
{

public:

  geometry_containment(collision_type collisionType) : m_type { collisionType }
  {
  }

  auto operator()(const default_object_geometry& containedObject, const default_object_geometry& containmentObject) const -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(const default_object_geometry &containedObject, const default_object_geometry &containmentObject) noexcept -> bool;

private:

  collision_type m_type;

};
