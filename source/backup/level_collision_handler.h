#pragma once

#include "default_object.h"
#include "play_events.h"

class level_collision_handler
{

public:

  level_collision_handler();

  auto operator()(default_object& object1, default_object& object2) -> void;
  auto operator()(default_object& object, particle& particle) -> void;
  auto operator()(default_object& object) -> void;
  auto operator()(particle& particle) -> void;

  [[nodiscard]] auto ExitCell() -> std::optional<POINT_2I>;

private:

  template <typename object_type_1, typename object_type_2> auto OnCollision(default_object& object1, default_object& object2) -> void;
  template <typename object_type> auto OnCollision(default_object& object, const level_cell_item& cell) -> void;

  auto OnCollision(player_bullet& bullet, enemy_ship& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_3& enemy) -> void;

  auto OnCollision(player_ship& playerShip, enemy_ship& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_3& enemy) -> void;

  auto OnCollision(player_ship& playerShip, enemy_bullet& enemyBullet) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;

  auto OnCollision(player_ship& ship, level_cell& wall) -> void;
  auto OnCollision(player_bullet& bullet, level_cell& wall) -> void;
  auto OnCollision(enemy_bullet& bullet, level_cell& wall) -> void;

  auto OnCollision(auto& object, particle& particle) -> void;
  auto OnCollision(auto&& object, const level_cell_item& cell) -> void;
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

auto level_collision_handler::OnCollision(auto&& object, const level_cell_item& cell) -> void
{
  object.Destroy();
}

auto level_collision_handler::OnCollision(auto&& object1, auto&& object2) -> void
{
  object1.Destroy();
  object2.Destroy();
}
