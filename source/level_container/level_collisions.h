#pragma once

#include "collisions/geometry_collision_unary.h"
#include "collisions/geometry_collision_binary.h"
#include "collisions/particle_collision.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"

template <typename visitor_type>
class level_collision_handler
{

public:

  level_collision_handler(visitor_type& visitor) : m_visitor { visitor }
  {
  }

  auto operator()(default_object& object1, default_object& object2) -> void;
  auto operator()(default_object& object, particle& particle) -> void;
  auto operator()(default_object& object) -> void;
  auto operator()(particle& particle) -> void;

private:

  visitor_type& m_visitor;

};

template <typename visitor_type>
auto level_collision_handler<visitor_type>::operator()(default_object& object1, default_object& object2) -> void
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

template <typename visitor_type>
auto level_collision_handler<visitor_type>::operator()(default_object& object, particle& particle) -> void
{
  auto& objectVar = object.Get();

  auto* mineObject = std::get_if<mine>(&objectVar);

  if( mineObject )
  {
    if( mineObject->Type() == mine::type::one && particle.Type() == particle::type::bullet_two || 
        mineObject->Type() == mine::type::two && particle.Type() == particle::type::bullet_one )
    {
      m_visitor.MineDestroyed(object.Position());
      object.Destroy();
    }
  }

  auto* playerShipObject = std::get_if<player_ship>(&objectVar);

  if( playerShipObject == nullptr || !particle.IsBullet() )
  {
    particle.Destroy();
  }
}

template <typename visitor_type>
auto level_collision_handler<visitor_type>::operator()(default_object& object) -> void
{
  m_visitor.CreateExplosion(object.Position());
  object.Destroy();
}

template <typename visitor_type>
auto level_collision_handler<visitor_type>::operator()(particle& particle) -> void
{
  m_visitor.CreateImpact(particle.Position());
  particle.Destroy();
}
