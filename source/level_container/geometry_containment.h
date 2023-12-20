#pragma once

#include "dynamic_object.h"

template <typename contained_object_type>
class geometry_containment
{

public:

  geometry_containment(auto&& callable) : m_callable { callable }
  {
  }

  auto operator()(ID2D1Geometry* containmentGeometry, dynamic_object<contained_object_type>& containedObject) -> void
  {
    D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
    HRESULT hr = containmentGeometry->CompareWithGeometry(containedObject.Geometry().Get(), D2D1::Matrix3x2F::Identity(), &relation);

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

    if( !contained )
    {
      m_callable(containedObject);
    }
  }

  auto operator()(ID2D1Geometry* containmentGeometry, dynamic_object_collection<contained_object_type>& containedObjectCollection) -> void
  {
    for( auto& containedObject : containedObjectCollection )
    {
      (*this)(containmentGeometry, containedObject);
    }
  }

private:

  std::function<void(dynamic_object<contained_object_type>&)> m_callable;

};
