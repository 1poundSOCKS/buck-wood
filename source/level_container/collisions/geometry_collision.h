#pragma once

#include "game_settings.h"

class geometry_collision
{

public:

  auto operator()(auto&& object1, auto&& object2, auto&& callable) -> void
  {
    auto position1 = object1->Position();
    auto position2 = object2->Position();

    // auto possibleCollision = direct2d::GetDistanceBetweenPoints(position1, position2) < object1.GeometryRadius() + object2.GeometryRadius();
    auto possibleCollision = direct2d::CheckOverlap(object1.GeometryBounds(), object2.GeometryBounds());

    switch( game_settings::collisionDetectionType() )
    {
      case game_settings::collision_detection_type::direct2d:
        if( possibleCollision && CheckDirect2D(object1, object2) )
        {
          callable(object1.Object(), object2.Object());
        }
        break;
      case game_settings::collision_detection_type::basic:
        if( possibleCollision )
        {
          callable(object1.Object(), object2.Object());
        }
        break;
    }
  }

  [[nodiscard]] auto CheckDirect2D(auto&& object1, auto&& object2) const noexcept -> bool
  {
    D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
    HRESULT hr = object1.Geometry()->CompareWithGeometry(object2.Geometry(), D2D1::Matrix3x2F::Identity(), &relation);

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

};
