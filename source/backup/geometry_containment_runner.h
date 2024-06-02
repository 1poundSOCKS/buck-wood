#pragma once

#include "geometry_containment.h"

class geometry_containment_runner
{

public:

  geometry_containment_runner(collision_type collisionType) : m_geometryContainment { collisionType }
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
          m_geometryContainment(object1, object2, callable);
        }
      }
    });
  }

private:

  std::mutex m_mutex;
  geometry_containment m_geometryContainment;

};
