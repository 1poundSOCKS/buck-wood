#pragma once

#include "collision_type.h"
#include "collision_object.h"

class geometry_collision
{

public:

  geometry_collision(collision_type collsionType);
  auto operator()(collision_object& object1, collision_object& object2) -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(collision_object& object1, collision_object& object2) noexcept -> bool;

private:

  collision_type m_type;

};
