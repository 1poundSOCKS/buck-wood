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

  template <typename object_type_1, typename object_type_2> auto OnCollision(default_object& object1, default_object& object2) -> void;
  auto OnCollision(player_bullet& playerBullet, mine& mine) -> void;
  auto OnCollision(player_bullet& playerBullet, level_target& levelTarget) -> void;
  auto OnCollision(player_ship& playerShip, mine& mine) -> void;
  auto OnCollision(player_ship& playerShip, level_target& target) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;
  auto OnCollision(auto& object, particle& particle) -> void;

private:

  level_container& m_visitor;

};

auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  OnCollision<player_bullet, mine>(object1, object2);
  OnCollision<player_bullet, level_target>(object1, object2);
  OnCollision<player_ship, mine>(object1, object2);
  OnCollision<player_ship, level_target>(object1, object2);
  OnCollision<player_ship, power_up>(object1, object2);
}

auto level_collision_handler::operator()(default_object& object, particle& particle) -> void
{
  std::visit([this, &particle](auto& object){ OnCollision(object, particle); }, object.Get());
}

template <typename object_type_1, typename object_type_2>
auto level_collision_handler::OnCollision(default_object& object1, default_object& object2) -> void
{
  if( std::holds_alternative<object_type_1>(object1.Get()) && std::holds_alternative<object_type_2>(object2.Get()) )
  {
    return OnCollision(std::get<object_type_1>(object1.Get()), std::get<object_type_2>(object2.Get()));
  }

  if( std::holds_alternative<object_type_1>(object2.Get()) && std::holds_alternative<object_type_2>(object1.Get()) )
  {
    return OnCollision(std::get<object_type_1>(object2.Get()), std::get<object_type_2>(object1.Get()));
  }
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
  switch( particle.Type() )
  {
    case particle::type::explosion:
      m_visitor.CreateImpact(particle.Position());
      particle.Destroy();
      break;
  }
}

auto level_collision_handler::OnCollision(player_bullet& playerBullet, mine& mine) -> void
{
  m_visitor.CreatePowerUp(mine.Position(), mine.Angle());
  m_visitor.MineDestroyed();
  playerBullet.Destroy();
  mine.Destroy();
}

auto level_collision_handler::OnCollision(player_bullet& playerBullet, level_target& levelTarget) -> void
{
  m_visitor.CreateExplosion(playerBullet.Position());
  playerBullet.Destroy();
  levelTarget.ApplyDamage(playerBullet.Damage());
  m_visitor.TargetDamaged(levelTarget);
}

auto level_collision_handler::OnCollision(player_ship& playerShip, mine& mine) -> void
{
  m_visitor.CreateExplosion(playerShip.Position());
  playerShip.Destroy();
  mine.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, level_target& levelTarget) -> void
{
  m_visitor.CreateExplosion(playerShip.Position());
  playerShip.Destroy();
  levelTarget.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  m_visitor.PowerUpCollected(powerUp.Position());
  powerUp.Destroy();
}
