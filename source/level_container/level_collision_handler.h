#pragma once

// #include "collisions/geometry_collision_unary.h"
// #include "collisions/geometry_collision_binary.h"
// #include "collisions/particle_collision.h"
// #include "collisions/geometry_containment.h"
// #include "collisions/particle_containment.h"
// #include "level_container.h"
#include "level_objects.h"
// #include "play_events.h"

class level_collision_handler
{

public:

  using explosion_collection = std::vector<POINT_2F>;
  using impact_collection = std::vector<POINT_2F>;

  // level_collision_handler(level_container& visitor) : m_visitor { visitor }
  // level_collision_handler(std::shared_ptr<level_container> visitor) : m_visitor { visitor }
  // level_collision_handler(std::shared_ptr<play_events> playEvents) : m_playEvents { playEvents }
  level_collision_handler()
  {
    m_explosions.reserve(10);
    m_impacts.reserve(100);
  }

  auto reset() -> void
  {
    m_explosions.clear();
    m_impacts.clear();
  }

  // auto SetVisitor(std::shared_ptr<level_container> visitor) -> void;

  [[nodiscard]] auto Explosions() const -> const explosion_collection&
  {
    return m_explosions;
  }

  [[nodiscard]] auto Impacts() const -> const impact_collection&
  {
    return m_impacts;
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

  // std::shared_ptr<level_container> m_visitor;
  explosion_collection m_explosions;
  impact_collection m_impacts;
  // std::shared_ptr<play_events> m_playEvents;

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

// auto level_collision_handler::SetVisitor(std::shared_ptr<level_container> visitor) -> void
// {
//   m_visitor = visitor;
// }

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
  particle.Destroy();
}

inline auto level_collision_handler::operator()(default_object& object) -> void
{
  // m_visitor->CreateExplosion(object.Position());
  m_explosions.emplace_back(object.Position());
  object.Destroy();
}

inline auto level_collision_handler::operator()(particle& particle) -> void
{
  switch( particle.Type() )
  {
    case particle::type::explosion:
      // m_visitor->CreateImpact(particle.Position());
      m_impacts.emplace_back(particle.Position());
      particle.Destroy();
      break;
  }
}

inline auto level_collision_handler::OnCollision(player_bullet& playerBullet, enemy_bullet_1& enemyBullet) -> void
{
  m_explosions.emplace_back(enemyBullet.Position());
  // m_playEvents->MineDestroyed();
  playerBullet.Destroy();
  enemyBullet.Destroy();
}

inline auto level_collision_handler::OnCollision(player_bullet& playerBullet, enemy_type_2& enemy) -> void
{
  m_explosions.emplace_back(playerBullet.Position());
  playerBullet.Destroy();
  enemy.ApplyDamage(playerBullet.Damage());
  // m_playEvents->TargetDamaged(enemy);
}

inline auto level_collision_handler::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void
{
  m_explosions.emplace_back(playerShip.Position());
  playerShip.Destroy();
  enemyBullet.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void
{
  m_explosions.emplace_back(playerShip.Position());
  playerShip.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  // m_playEvents->PowerUpCollected(powerUp.Position());
  powerUp.Destroy();
}

inline auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_1& enemy) -> void
{
  m_explosions.emplace_back(enemy.Position());
  ship.Destroy();
  enemy.Destroy();  
}

inline auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  m_explosions.emplace_back(enemy.Position());
  bullet.Destroy();
  enemy.Destroy();  
}
