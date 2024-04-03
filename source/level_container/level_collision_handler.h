#pragma once

#include "level_objects.h"
#include "play_events.h"
#include "level_container.h"

class level_collision_handler
{

public:

  level_collision_handler(level_container* levelContainer, std::shared_ptr<play_events> playEvents) : m_levelContainer { levelContainer }, m_playEvents { playEvents }
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

  level_container* m_levelContainer;
  std::shared_ptr<play_events> m_playEvents;

};

template <typename object_type_1, typename object_type_2> auto level_collision_handler::OnCollision(default_object& object1, default_object& object2) -> void
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