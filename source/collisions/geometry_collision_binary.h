#pragma once

#include "geometry_collision.h"

class geometry_collision_binary
{

public:

  geometry_collision_binary(collision_type collisionType) : m_geometryCollision { collisionType }
  {
  }

  auto operator()(std::ranges::input_range auto&& objectCollection1, std::ranges::input_range auto&& objectCollection2, auto&& callable) -> void
  {
    std::for_each(std::execution::par, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](auto& object1)
    {
      for( auto& object2 : objectCollection2 )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        if( !object1.Object().Destroyed() && !object2.Object().Destroyed() )
        {
          m_geometryCollision(object1, object2, callable);
        }
      }
    });
  }

private:

  std::mutex m_mutex;
  geometry_collision m_geometryCollision;

};
