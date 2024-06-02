#pragma once

#include "collision_type.h"
#include "collision_object.h"

class geometry_containment
{

public:

  geometry_containment(collision_type collisionType) : m_type { collisionType }
  {
  }

  auto operator()(collision_object& containedObject, collision_object& containmentObject) -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(collision_object &containedObject, collision_object &containmentObject) noexcept -> bool;

private:

  collision_type m_type;

};
