#include "pch.h"
#include "geometry_collision.h"

geometry_collision::geometry_collision(collision_type collisionType) : m_type { collisionType }
{
}

auto geometry_collision::operator()(const default_object_geometry &object1, const default_object_geometry &object2) const -> result
{
  auto bounds1 = object1.Bounds();
  auto bounds2 = object2.Bounds();
  
  auto overlapX = !(bounds1.left > bounds2.right || bounds1.right < bounds2.left);
  auto overlapY = !(bounds1.top > bounds2.bottom || bounds1.bottom < bounds2.top);
  
  auto boundaryCollision = overlapX && overlapY;

  switch( m_type )
  {
    case collision_type::direct2d:
      return boundaryCollision ? CheckDirect2D(object1, object2) : result::none;

    default:
      return boundaryCollision ? ( CheckBoundaryContainment(bounds1, bounds2) ? result::containment : result::collision ) : result::none;
  }
}

[[nodiscard]] auto geometry_collision::CheckDirect2D(const default_object_geometry& object1, const default_object_geometry& object2) noexcept -> result
{
  auto geometry1 = object1.Geometry().GetRaw();
  auto geometry2 = object2.Geometry().GetRaw();

  D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
  HRESULT hr = geometry1->CompareWithGeometry(geometry2, D2D1::Matrix3x2F::Identity(), &relation);

  auto collisionResult = result::none;

  if( SUCCEEDED(hr) )
  {
    switch( relation )
    {
      case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
        collisionResult = result::containment;
        break;
      case D2D1_GEOMETRY_RELATION_OVERLAP:
        collisionResult = result::collision;
        break;
    }
  }

  return collisionResult;
}

auto geometry_collision::CheckBoundaryContainment(RECT_F object1Bounds, RECT_F object2Bounds) const -> bool
{
  auto containedX = object2Bounds.right > object1Bounds.right && object2Bounds.left < object1Bounds.left;
  auto containedY = object2Bounds.top < object1Bounds.top && object2Bounds.bottom > object1Bounds.bottom;

  return containedX && containedY;
}
