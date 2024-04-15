#pragma once

#include "geometry_collision.h"

class geometry_collision_unary
{

public:

  geometry_collision_unary()
  {
  }

  auto operator()(auto&& object1, auto&& object2, auto&& callable) -> void
  {
    m_geometryCollision(object1, object2, callable);
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
          (*this)(object1, object2, callable);
        }
      });
    }
  }

private:

  std::mutex m_mutex;
  geometry_collision m_geometryCollision;

};
