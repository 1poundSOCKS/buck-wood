#pragma once

#include "collisions/geometry_collision_unary.h"
#include "collisions/geometry_collision_binary.h"
#include "collisions/particle_collision.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"
#include "level_container.h"

class level_collision_handler
{

public:

  level_collision_handler(level_container& visitor) : m_visitor { visitor }
  {
  }

  auto operator()(default_object& object1, default_object& object2) -> void;
  auto operator()(default_object& object, particle& particle) -> void;
  auto operator()(default_object& object) -> void;
  auto operator()(particle& particle) -> void;

private:

  auto OnCollision(player_bullet& playerBullet, mine& mine) -> void;
  auto OnCollision(player_bullet& playerBullet, level_target& levelTarget) -> void;
  auto OnCollision(player_ship& playerShip, mine& mine) -> void;
  auto OnCollision(auto& object, particle& particle) -> void;

private:

  level_container& m_visitor;

};

auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  if( std::holds_alternative<player_bullet>(object1.Get()) && std::holds_alternative<mine>(object2.Get()) )
  {
    return OnCollision(std::get<player_bullet>(object1.Get()), std::get<mine>(object2.Get()));
  }  

  if( std::holds_alternative<player_bullet>(object2.Get()) && std::holds_alternative<mine>(object1.Get()) )
  {
    return OnCollision(std::get<player_bullet>(object2.Get()), std::get<mine>(object1.Get()));
  }  

  if( std::holds_alternative<player_bullet>(object1.Get()) && std::holds_alternative<level_target>(object2.Get()) )
  {
    return OnCollision(std::get<player_bullet>(object1.Get()), std::get<level_target>(object2.Get()));
  }  

  if( std::holds_alternative<player_bullet>(object2.Get()) && std::holds_alternative<level_target>(object1.Get()) )
  {
    return OnCollision(std::get<player_bullet>(object2.Get()), std::get<level_target>(object1.Get()));
  }  

  if( std::holds_alternative<player_ship>(object1.Get()) && std::holds_alternative<mine>(object2.Get()) )
  {
    return OnCollision(std::get<player_ship>(object1.Get()), std::get<mine>(object2.Get()));
  }  

  if( std::holds_alternative<player_ship>(object2.Get()) && std::holds_alternative<mine>(object1.Get()) )
  {
    return OnCollision(std::get<player_ship>(object2.Get()), std::get<mine>(object1.Get()));
  }  
}

auto level_collision_handler::operator()(default_object& object, particle& particle) -> void
{
  std::visit([this, &particle](auto& object){ OnCollision(object, particle); }, object.Get());
}

auto level_collision_handler::OnCollision(auto& object, particle& particle) -> void
{
  particle.Destroy();
}

auto level_collision_handler::operator()(default_object& object) -> void
{
  m_visitor.CreateExplosion(object.Position());
  object.Destroy();
}

auto level_collision_handler::operator()(particle& particle) -> void
{
  m_visitor.CreateImpact(particle.Position());
  particle.Destroy();
}

auto level_collision_handler::OnCollision(player_bullet& playerBullet, mine& mine) -> void
{
  playerBullet.Destroy();
  mine.Destroy();
  m_visitor.CreateExplosion(mine.Position());
}

auto level_collision_handler::OnCollision(player_bullet& playerBullet, level_target& levelTarget) -> void
{
  playerBullet.Destroy();
  m_visitor.CreateExplosion(playerBullet.Position());
}

auto level_collision_handler::OnCollision(player_ship& playerShip, mine& mine) -> void
{
  playerShip.Destroy();
  mine.Destroy();
  m_visitor.CreateExplosion(playerShip.Position());
}
