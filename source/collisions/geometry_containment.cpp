#include "pch.h"
#include "geometry_containment.h"

auto geometry_containment::operator()(const default_object_geometry &containedObject, const default_object_geometry &containmentObject) const -> bool
{
  auto containedBounds = containedObject.Bounds();
  auto containerBounds = containmentObject.Bounds();

  auto containedX = containerBounds.right > containedBounds.right && containerBounds.left < containedBounds.left;
  auto containedY = containerBounds.top < containedBounds.top && containerBounds.bottom > containedBounds.bottom;

  auto boundaryContainment = containedX && containedY;

  switch( m_type )
  {
    case collision_type::direct2d:
      return boundaryContainment && CheckDirect2D(containedObject, containmentObject);

    case collision_type::boundary:
    default:
      return boundaryContainment;

  }
}

auto geometry_containment::CheckDirect2D(const default_object_geometry &containedObject, const default_object_geometry &containmentObject) noexcept -> bool
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
