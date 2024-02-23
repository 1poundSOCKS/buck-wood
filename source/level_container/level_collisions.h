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
    auto& objectVar = object.Get();

    auto* mineObject = std::get_if<mine>(&objectVar);

    if( mineObject )
    {
      if( mineObject->HardnessType() == mine::hardness_type::soft && particle.Type() == particle::type::bullet_two || 
          mineObject->HardnessType() == mine::hardness_type::tough && particle.Type() == particle::type::bullet_one )
      {
        m_visitor.CreateExplosion(object.Position());
        object.Destroy();
      }
    }

    auto* playerShipObject = std::get_if<player_ship>(&objectVar);

    if( playerShipObject == nullptr || !particle.IsBullet() )
    {
      particle.Destroy();
    }
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
