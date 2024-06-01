#pragma once

#include "collision_type.h"
#include "collision_object.h"

class geometry_collision
{

public:

  geometry_collision(collision_type collsionType);
  auto operator()(collision_object& object1, collision_object& object2, auto&& callable) -> void;
  auto operator()(collision_object& object1, collision_object& object2) -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(collision_object& object1, collision_object& object2) noexcept -> bool;

private:

  collision_type m_type;

};

inline auto geometry_collision::operator()(collision_object& object1, collision_object& object2, auto&& callable) -> void
{
  auto position1 = object1.Object().Position();
  auto position2 = object2.Object().Position();

  auto bounds1 = object1.Bounds();
  auto bounds2 = object2.Bounds();
  auto overlapX = !(bounds1.left > bounds2.right || bounds1.right < bounds2.left);
  auto overlapY = !(bounds1.top > bounds2.bottom || bounds1.bottom < bounds2.top);
  auto possibleCollision = overlapX && overlapY;

  switch( m_type )
  {
    case collision_type::direct2d:
      if( possibleCollision && CheckDirect2D(object1, object2) )
      {
        callable(object1.Object(), object2.Object());
      }
      break;

    case collision_type::boundary:
      if( possibleCollision )
      {
        callable(object1.Object(), object2.Object());
      }
      break;
  }
}
