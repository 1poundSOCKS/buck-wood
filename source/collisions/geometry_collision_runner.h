#pragma once

#include "range_comparision_runner.h"
#include "geometry_collision.h"

class geometry_collision_runner
{

public:

  geometry_collision_runner(range_comparison_runner::execution ex, collision_type collisionType) : m_compare(ex), m_collisionTest(collisionType)
  {
  }

  auto operator()(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, auto&& visitor);

private:

  range_comparison_runner m_compare;
  geometry_collision m_collisionTest;

};

auto geometry_collision_runner::operator()(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, auto &&visitor)
{
  m_compare(objectGeometries1, objectGeometries2, [this,&visitor](auto& object1, auto& object2)
  {
    auto collisionResult = m_collisionTest(object1, object2);

    object1.Object().Visit([this,&object2,&visitor,collisionResult](auto& levelObject1)
    {
      object2.Object().Visit([this,&levelObject1,&visitor,collisionResult](auto& levelObject2) { visitor(levelObject1, levelObject2, collisionResult); } );
    });
  });
}
