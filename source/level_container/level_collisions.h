#pragma once

#include "collisions/geometry_collision.h"
#include "collisions/particle_collision.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"
#include "collisions/collision.h"

#include "level_objects.h"

template <typename visitor_type>
class level_collision_handler
{

public:

  level_collision_handler(visitor_type& visitor) : m_visitor { visitor }
  {
  }

  auto operator()(player_ship& ship, mine& mine) -> void
  {
    if( ship.ShieldsUp() && mine.HardnessType() == mine::hardness_type::soft || !ship.ShieldsUp() && mine.HardnessType() == mine::hardness_type::tough )
    {      
      ship.ApplyFatalDamage();
    }

    mine.Destroy();
    m_visitor.CreateExplosion(mine.PreviousPosition());
  }

  auto operator()(player_ship& ship, level_target& target) -> void
  {
    ship.ApplyFatalDamage();
    m_visitor.CreateExplosion(ship.PreviousPosition());
  }

private:

  visitor_type& m_visitor;

};
