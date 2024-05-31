#pragma once

#include "geometry_collision.h"

class geometry_collision_unary
{

public:

  geometry_collision_unary(collision_type collisionType) : m_geometryCollision { collisionType }
  {
  }

  auto operator()(std::ranges::input_range auto&& objectCollection, auto&& callable) -> void
  {
    for( auto objectIt = std::begin(objectCollection); objectIt != std::end(objectCollection); ++objectIt )
    {
      auto& object1 = *objectIt;
      
      std::for_each(std::execution::par, std::next(objectIt), std::end(objectCollection), [this, &object1, &callable](auto& object2)
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        if( !object1->Destroyed() && !object2->Destroyed() )
        {
          m_geometryCollision(object1, object2, callable);
        }
      });
    }
  }

private:

  std::mutex m_mutex;
  geometry_collision m_geometryCollision;

};
