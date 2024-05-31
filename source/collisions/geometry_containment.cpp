#include "pch.h"
#include "geometry_containment.h"

auto geometry_containment::CheckDirect2D(collision_object &containedObject, collision_object &containmentObject) noexcept -> bool
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
