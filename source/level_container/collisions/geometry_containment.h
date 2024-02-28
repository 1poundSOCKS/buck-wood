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
    HRESULT hr = containmentGeometry->CompareWithGeometry(containedObject.Geometry(), D2D1::Matrix3x2F::Identity(), &relation);

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
      std::lock_guard<std::mutex> guard(m_mutex);
      
      if( !containedObject.Object().Destroyed() )
      {
        m_callable(containedObject.Object());
      }
    }
  }

  auto operator()(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& objectCollection) -> void
  {
    std::for_each(std::execution::par, std::begin(objectCollection), std::end(objectCollection), [this,containmentGeometry](auto& object)
    {
      (*this)(containmentGeometry, object);
    });
  }

private:

  std::function<void(contained_object_type&)> m_callable;
  std::mutex m_mutex;

};
