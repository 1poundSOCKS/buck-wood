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
      m_callable(containedObject);
    }
  }

  auto operator()(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& objectCollection) -> void
  {
    auto activeObjects = std::ranges::views::filter(objectCollection, [](const auto& object) { return object->Destroyed() ? false : true; });

    std::for_each(std::execution::par_unseq, std::begin(activeObjects), std::end(activeObjects), [this,containmentGeometry](auto& object)
    {
      (*this)(containmentGeometry, object);
    });
  }

private:

  std::function<void(dynamic_object<contained_object_type>&)> m_callable;
  std::mutex m_mutex;

};
