#pragma once

#include "range_comparision_runner.h"
#include "geometry_collision.h"
#include "geometry_containment.h"

class geometry_collision_runner
{

public:

  enum class check_type { collision_only, containment_only, collision_and_containment };
  enum class result_type { collision, containment };

  geometry_collision_runner(collision_type collisionType) : m_collisionTest { collisionType }, m_containmentTest { collisionType }
  {
  }

  auto operator()(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, check_type checkType, auto&& visitor);

private:

  range_comparison_runner m_compare;
  geometry_collision m_collisionTest;
  geometry_containment m_containmentTest;

};

auto geometry_collision_runner::operator()(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, check_type checkType, auto &&visitor)
{
  auto checkCollision = ( checkType == check_type::collision_only || checkType == check_type::collision_and_containment );
  auto checkContainment = ( checkType == check_type::containment_only || checkType == check_type::collision_and_containment );

  m_compare(objectGeometries1, objectGeometries2, [this,checkCollision,checkContainment,&visitor](auto& object1, auto& object2)
  {
    if( checkCollision && m_collisionTest(object1, object2) )
    {
      object1.Object().Visit([this,&object2,&visitor](auto& levelObject1)
      {
        object2.Object().Visit([this,&levelObject1,&visitor](auto& levelObject2) { visitor(levelObject1, levelObject2, result_type::collision); } );
      });
    }

    if( checkContainment && m_containmentTest(object1, object2) )
    {
      object1.Object().Visit([this,&object2,&visitor](auto& levelObject1)
      {
        object2.Object().Visit([this,&levelObject1,&visitor](auto& levelObject2) { visitor(levelObject1, levelObject2, result_type::containment); } );
      });
    }
  });
}
