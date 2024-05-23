#pragma once

#include "level_objects.h"
#include "play_events.h"

class level_collision_handler
{

public:

  level_collision_handler();

  auto operator()(default_object& object1, default_object& object2) -> void;
  auto operator()(default_object& object, particle& particle) -> void;
  auto operator()(default_object& object, const valid_cell& cell) -> void;
  auto operator()(default_object& object) -> void;
  auto operator()(particle& particle) -> void;
  [[nodiscard]] auto ExitCell() -> std::optional<POINT_2I>;

private:

  template <typename object_type_1, typename object_type_2> auto OnCollision(default_object& object1, default_object& object2) -> void;
  template <typename object_type> auto OnCollision(default_object& object, const valid_cell& cell) -> void;
  auto OnCollision(player_bullet& playerBullet, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void;
  auto OnCollision(player_bullet& playerBullet, enemy_type_2& enemy) -> void;
  auto OnCollision(player_missile& missile, enemy_type_1& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;
  auto OnCollision(player_ship& ship, enemy_type_1& enemy) -> void;
  auto OnCollision(player_ship& ship, const valid_cell& cell) -> void;
  auto OnCollision(player_ship& ship, level_wall& wall) -> void;
  auto OnCollision(player_bullet& bullet, level_wall& wall) -> void;
  auto OnCollision(enemy_bullet_1& bullet, level_wall& wall) -> void;
  auto OnCollision(auto& object, particle& particle) -> void;
  auto OnCollision(auto&& object, const valid_cell& cell) -> void;
  auto OnCollision(auto&& object1, auto&& object2) -> void;

private:

  std::optional<POINT_2I> m_exitCell;

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

template <typename object_type> auto level_collision_handler::OnCollision(default_object& object, const valid_cell& cell) -> void
{
  if( std::holds_alternative<object_type>(object.Get()) )
  {
    OnCollision(std::get<object_type>(object.Get()), cell);
  }
}

auto level_collision_handler::OnCollision(auto&& object, const valid_cell& cell) -> void
{
  object.Destroy();
}

auto level_collision_handler::OnCollision(auto&& object1, auto&& object2) -> void
{
  object1.Destroy();
  object2.Destroy();
}
