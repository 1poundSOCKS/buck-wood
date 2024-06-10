#include "pch.h"
#include "level_collision_handler.h"
#include "player_state.h"

level_collision_handler::level_collision_handler()
{
}

auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  OnCollision<player_bullet, enemy_type_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_2>(object1, object2);
  OnCollision<player_bullet, enemy_type_3>(object1, object2);
  
  OnCollision<player_ship, enemy_type_1>(object1, object2);
  OnCollision<player_ship, enemy_type_2>(object1, object2);
  OnCollision<player_ship, enemy_type_3>(object1, object2);

  OnCollision<player_ship, enemy_bullet_1>(object1, object2);
  OnCollision<player_ship, power_up>(object1, object2);
  
  OnCollision<player_ship, level_cell>(object1, object2);
  OnCollision<player_bullet, level_cell>(object1, object2);
  OnCollision<enemy_bullet_1, level_cell>(object1, object2);
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
  object.Destroy();
}

auto level_collision_handler::operator()(particle& particle) -> void
{
  switch( particle.Type() )
  {
    case particle::type::explosion:
      particle.Destroy();
      break;
  }
}

auto level_collision_handler::ExitCell() -> std::optional<POINT_2I>
{
  return m_exitCell;
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  enemy.ApplyDamage(bullet.Damage());
  bullet.Destroy();
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_3& enemy) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_1& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_2& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_3& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      playerShip.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemyBullet.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  powerUp.Destroy();
}

auto level_collision_handler::OnCollision(player_ship &ship, level_cell &wall) -> void
{
  switch( wall.Type() )
  {
    case level_cell_type::exit:
      m_exitCell = wall.CellId();
      break;
  }
}

auto level_collision_handler::OnCollision(player_bullet& bullet, level_cell& wall) -> void
{
  switch( wall.Type() )
  {
    case level_cell_type::wall:
    case level_cell_type::exit:
      bullet.Destroy();
      break;
  }
}

auto level_collision_handler::OnCollision(enemy_bullet_1 &bullet, level_cell &wall) -> void
{
  switch( wall.Type() )
  {
    case level_cell_type::wall:
    case level_cell_type::exit:
      bullet.Destroy();
      break;
  }
}
