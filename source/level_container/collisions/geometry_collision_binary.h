#pragma once

class geometry_collision_binary
{

public:

  geometry_collision_binary()
  {
  }

  auto operator()(auto&& object1, auto&& object2, auto&& callable) -> void
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

  auto operator()(auto&& object, std::ranges::input_range auto&& objectCollection, auto&& callable) -> void
  {
    if( !object->Destroyed() )
    {
      auto activeObjects = std::ranges::views::filter(objectCollection, [](const auto& object) { return object->Destroyed() ? false : true; });

      std::for_each(std::execution::par, std::begin(objectCollection), std::end(objectCollection), [this,&object,&callable](auto& collectionObject)
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        (*this)(object, collectionObject, callable);
      });
    }
  }

  auto operator()(std::ranges::input_range auto&& objectCollection1, std::ranges::input_range auto&& objectCollection2, auto&& callable) -> void
  {
    std::for_each(std::execution::par, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](auto& object1)
    {
      for( auto& object2 : objectCollection2 )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        if( !object1->Destroyed() && !object2->Destroyed() )
        {
          (*this)(object1, object2, callable);
        }
      }
    });
  }

private:

  std::mutex m_mutex;

};
