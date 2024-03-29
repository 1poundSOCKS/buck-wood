#pragma once

#include "dynamic_object.h"

template <typename object_type>
class geometry_collision_unary
{

public:

  // geometry_collision_unary(auto&& callable) : m_callable { callable }
  geometry_collision_unary()
  {
  }

  auto operator()(dynamic_object<object_type>& object1, dynamic_object<object_type>& object2, auto&& callable) -> void
  {
    auto position1 = object1->Position();
    auto position2 = object2->Position();

    if( direct2d::GetDistanceBetweenPoints(position1, position2) < object1.GeometryRadius() + object2.GeometryRadius() )
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

      if( collided )
      {
        callable(object1.Object(), object2.Object());
      }
    }
  }

  auto operator()(std::ranges::input_range auto&& objectCollection, auto&& callable) -> void
  {
    for( auto objectIt = std::begin(objectCollection); objectIt != std::end(objectCollection); ++objectIt )
    {
      auto& object1 = *objectIt;
      
      std::for_each(std::execution::seq, std::next(objectIt), std::end(objectCollection), [this, &object1, &callable](auto& object2)
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        if( !object1->Destroyed() && !object2->Destroyed() )
        {
          (*this)(object1, object2, callable);
        }
      });
    }
  }

private:

  // std::function<void(object_type&, object_type&)> m_callable;
  std::mutex m_mutex;

};
