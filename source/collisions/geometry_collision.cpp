#include "pch.h"
#include "geometry_collision.h"

geometry_collision::geometry_collision(collision_type collisionType) : m_type { collisionType }
{
}

auto geometry_collision::operator()(default_object_geometry &object1, default_object_geometry &object2) -> bool
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
      return possibleCollision && CheckDirect2D(object1, object2);

    case collision_type::boundary:
    default:
      return possibleCollision;
  }
}

[[nodiscard]] auto geometry_collision::CheckDirect2D(default_object_geometry& object1, default_object_geometry& object2) noexcept -> bool
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
