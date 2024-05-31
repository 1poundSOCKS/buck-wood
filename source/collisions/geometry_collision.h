#pragma once

// #include "game_settings.h"
#include "collision_type.h"
#include "collision_object.h"

class geometry_collision
{

public:

  geometry_collision(collision_type collsionType) : m_type { collsionType }
  {
  }

  auto operator()(collision_object& object1, collision_object& object2, auto&& callable) -> void
  {
    auto position1 = object1.Object().Position();
    auto position2 = object2.Object().Position();

    auto bounds1 = object1.Bounds();
    auto bounds2 = object2.Bounds();
    auto overlapX = !(bounds1.left > bounds2.right || bounds1.right < bounds2.left);
    auto overlapY = !(bounds1.top > bounds2.bottom || bounds1.bottom < bounds2.top);
    auto possibleCollision = overlapX && overlapY;

    // switch( game_settings::collisionDetectionType() )
    switch( m_type )
    {
      // case game_settings::collision_detection_type::direct2d:
      case collision_type::direct2d:
        if( possibleCollision && CheckDirect2D(object1, object2) )
        {
          callable(object1.Object(), object2.Object());
        }
        break;

      // case game_settings::collision_detection_type::basic:
      case collision_type::boundary:
        if( possibleCollision )
        {
          callable(object1.Object(), object2.Object());
        }
        break;
    }
  }

  [[nodiscard]] auto CheckDirect2D(collision_object& object1, collision_object& object2) const noexcept -> bool
  {
    auto geometry1 = object1.Geometry().GetRaw();
    auto geometry2 = object2.Geometry().GetRaw();

    D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
    HRESULT hr = geometry1->CompareWithGeometry(geometry2, D2D1::Matrix3x2F::Identity(), &relation);

    bool collided = false;

    if( SUCCEEDED(hr) )
    {
      switch( relation )
      {
        case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
        case D2D1_GEOMETRY_RELATION_CONTAINS:
        case D2D1_GEOMETRY_RELATION_OVERLAP:
          collided = true;
          break;
      }
    }

    return collided;
  }

  collision_type m_type;

};
