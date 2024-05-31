#include "pch.h"
#include "geometry_containment.h"

auto geometry_containment::operator()(collision_object &containedObject, collision_object &containmentObject, auto &&callable) -> void
{
  auto containedPosition = containedObject.Object().Position();
  auto containmentPosition = containmentObject.Object().Position();

  auto containedBounds = containedObject.Bounds();
  auto containerBounds = containmentObject.Bounds();

  auto containedX = containedBounds.left < containerBounds.right && containedBounds.right > containerBounds.left;
  auto containedY = containedBounds.top < containerBounds.bottom && containedBounds.bottom > containerBounds.top;
  auto possibleCollision = containedX && containedY;

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

auto geometry_containment::CheckDirect2D(collision_object &containedObject, collision_object &containmentObject) -> bool
{
  auto containedGeometry = containedObject.Geometry().GetRaw();
  auto containmentGeometry = containmentObject.Geometry().GetRaw();

  D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
  HRESULT hr = containmentGeometry->CompareWithGeometry(containedGeometry, D2D1::Matrix3x2F::Identity(), &relation);

  bool contained = false;

  if( SUCCEEDED(hr) )
  {
    switch( relation )
    {
      case D2D1_GEOMETRY_RELATION_CONTAINS:
        contained = true;
        break;
    }
  }

  return !contained;
}
