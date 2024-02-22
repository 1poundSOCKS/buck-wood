#pragma once

#include "collisions/geometry_collision.h"
#include "collisions/particle_collision.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"
#include "collisions/collision.h"

template <typename visitor_type>
class level_collision_handler
{

public:

  level_collision_handler(visitor_type& visitor) : m_visitor { visitor }
  {
  }

  auto operator()(default_object& object1, default_object& object2) -> void
  {
    if( !(std::holds_alternative<mine>(object1.Get()) && std::holds_alternative<level_target>(object2.Get()) ||
        std::holds_alternative<level_target>(object1.Get()) && std::holds_alternative<mine>(object2.Get())) )
    {
      object1.Destroy();
      object2.Destroy();
    }

    if( object1.Destroyed() ) m_visitor.CreateExplosion(object1.Position());
    if( object2.Destroyed() ) m_visitor.CreateExplosion(object2.Position());
  }

  auto operator()(default_object& object, particle& particle) -> void
  {
    particle.Destroy();

    // if( mine->HardnessType() == mine::hardness_type::soft && bullet.DamageMode() == bullet::damage_mode::two || 
    //     mine->HardnessType() == mine::hardness_type::tough && bullet.DamageMode() == bullet::damage_mode::one )
    // {
      // m_visitor.CreateExplosion(object->Position());
      // object.Destroy();
      // m_visitor.MineDestroyed();
    // }
  }

  auto operator()(default_object& object)
  {
    m_visitor.CreateExplosion(object.Position());
    object.Destroy();
  }

  auto operator()(particle& particle)
  {
    m_visitor.CreateImpact(particle.Position());
    particle.Destroy();
  }

private:

  visitor_type& m_visitor;

};
