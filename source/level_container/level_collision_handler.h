#pragma once

#include "level_objects.h"
#include "play_events.h"
#include "level_container.h"

class level_collision_handler
{

public:

  using explosion_collection = std::vector<POINT_2F>;
  using impact_collection = std::vector<POINT_2F>;

  level_collision_handler(std::shared_ptr<level_container> levelContainer, std::shared_ptr<play_events> playEvents) : m_levelContainer { levelContainer }, m_playEvents { playEvents }
  {
  }

  auto operator()(default_object& object1, default_object& object2) -> void;
  auto operator()(default_object& object, particle& particle) -> void;
  auto operator()(default_object& object) -> void;
  auto operator()(particle& particle) -> void;

private:

  template <typename object_type_1, typename object_type_2> auto OnCollision(default_object& object1, default_object& object2) -> void;
  auto OnCollision(player_bullet& playerBullet, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_bullet& playerBullet, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;
  auto OnCollision(player_ship& ship, enemy_type_1& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void;
  auto OnCollision(auto& object, particle& particle) -> void;

private:

  std::shared_ptr<level_container> m_levelContainer;
  std::shared_ptr<play_events> m_playEvents;

};

inline auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  OnCollision<player_bullet, enemy_bullet_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_2>(object1, object2);
  OnCollision<player_ship, enemy_bullet_1>(object1, object2);
  OnCollision<player_ship, enemy_type_2>(object1, object2);
  OnCollision<player_ship, power_up>(object1, object2);
  OnCollision<player_ship, enemy_type_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_1>(object1, object2);
}

inline auto level_collision_handler::operator()(default_object& object, particle& particle) -> void
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
  m_levelContainer->CreateImpact(particle.Position());
  particle.Destroy();
}

inline auto level_collision_handler::operator()(default_object& object) -> void
{
  m_levelContainer->CreateExplosion(object.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  object.Destroy();
}

inline auto level_collision_handler::operator()(particle& particle) -> void
{
  switch( particle.Type() )
  {
    case particle::type::explosion:
      m_levelContainer->CreateImpact(particle.Position());
      particle.Destroy();
      break;
  }
}

inline auto level_collision_handler::OnCollision(player_bullet& playerBullet, enemy_bullet_1& enemyBullet) -> void
{
  m_levelContainer->CreateExplosion(enemyBullet.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  playerBullet.Destroy();
  enemyBullet.Destroy();
}

inline auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

inline auto level_collision_handler::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void
{
  m_levelContainer->CreateExplosion(playerShip.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  playerShip.Destroy();
  enemyBullet.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_2& enemy) -> void
{
  m_levelContainer->CreateExplosion(ship.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  ship.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  m_playEvents->SetEvent(play_events::event_type::power_up_collected, true);
  powerUp.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_1& enemy) -> void
{
  m_levelContainer->CreateExplosion(ship.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  ship.Destroy();
  enemy.Destroy();
}

inline auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  m_playEvents->SetEvent(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.Destroy();  
}
