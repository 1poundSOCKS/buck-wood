#pragma once

#include "dynamic_object.h"

template <typename object_type_1, typename object_type_2>
class geometry_collision
{

public:

  geometry_collision(auto&& callable) : m_callable { callable }
  {
  }

  auto operator()(dynamic_object<object_type_1>& object1, dynamic_object<object_type_2>& object2) -> void
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
        m_callable(object1.Object(), object2.Object());
      }
    }
  }

  auto operator()(dynamic_object<object_type_1>& object, std::ranges::input_range auto&& objectCollection) -> void
  {
    if( !object->Destroyed() )
    {
      auto activeObjects = std::ranges::views::filter(objectCollection, [](const auto& object) { return object->Destroyed() ? false : true; });

      std::for_each(std::execution::par_unseq, std::begin(objectCollection), std::end(objectCollection), [this,&object](auto& collectionObject)
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        (*this)(object, collectionObject);
      });
    }
  }

  auto operator()(std::ranges::input_range auto&& objectCollection1, std::ranges::input_range auto&& objectCollection2) -> void
  {
    auto activeObjects1 = std::ranges::views::filter(objectCollection1, [](const auto& object) { return object->Destroyed() ? false : true; });
    auto activeObjects2 = std::ranges::views::filter(objectCollection2, [](const auto& object) { return object->Destroyed() ? false : true; });

    std::for_each(std::execution::par_unseq, std::begin(activeObjects1), std::end(activeObjects1), [this,&activeObjects2](auto& object1)
    {
      for( auto& object2 : activeObjects2 )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        (*this)(object1, object2);
      }
    });
  }

private:

  std::function<void(object_type_1&, object_type_2&)> m_callable;
  std::mutex m_mutex;

};
