#include "pch.h"
#include "level_collision_handler.h"

level_collision_handler::level_collision_handler(level_container* levelContainer) : m_levelContainer { levelContainer }
{
}

auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  OnCollision<player_bullet, enemy_bullet_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_2>(object1, object2);
  OnCollision<player_ship, enemy_bullet_1>(object1, object2);
  OnCollision<player_ship, enemy_type_2>(object1, object2);
  OnCollision<player_ship, power_up>(object1, object2);
  OnCollision<player_ship, enemy_type_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_1>(object1, object2);
  OnCollision<player_missile, enemy_type_1>(object1, object2);
  OnCollision<player_missile, enemy_type_2>(object1, object2);
}

auto level_collision_handler::operator()(default_object& object, particle& particle) -> void
{
  std::visit([this, &particle](auto& object){ OnCollision(object, particle); }, object.Get());
}

auto level_collision_handler::OnCollision(auto& object, particle& particle) -> void
{
  m_levelContainer->CreateImpact(particle.Position());
  particle.Destroy();
}

auto level_collision_handler::operator()(default_object& object) -> void
{
  m_levelContainer->CreateExplosion(object.Position());
  play_events::set(play_events::event_type::explosion, true);
  object.Destroy();
}

auto level_collision_handler::operator()(particle& particle) -> void
{
  switch( particle.Type() )
  {
    case particle::type::explosion:
      m_levelContainer->CreateImpact(particle.Position());
      particle.Destroy();
      break;
  }
}

auto level_collision_handler::OnCollision(player_bullet& playerBullet, enemy_bullet_1& enemyBullet) -> void
{
  m_levelContainer->CreateExplosion(enemyBullet.Position());
  play_events::set(play_events::event_type::explosion, true);
  playerBullet.Destroy();
  enemyBullet.Destroy();
  play_events::increment(play_events::counter_type::bullets_destroyed);
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  play_events::set(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_collision_handler::OnCollision(player_missile& missile, enemy_type_1& enemy) -> void
{
  enemy.ApplyDamage(missile.Damage());
  missile.Destroy();
  m_levelContainer->CreateExplosion(enemy.Destroyed() ? enemy.Position() : missile.Position());
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  play_events::set(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_collision_handler::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void
{
  if( m_levelContainer->PlayerInvulnerable() )
  {
    m_levelContainer->CreateExplosion(enemyBullet.Position());
  }
  else
  {
    m_levelContainer->CreateExplosion(playerShip.Position());
    play_events::set(play_events::event_type::explosion, true);
    playerShip.Destroy();
  }

  enemyBullet.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_2& enemy) -> void
{
  if( m_levelContainer->PlayerInvulnerable() )
  {
    m_levelContainer->CreateExplosion(enemy.Position());
  }
  else
  {
    m_levelContainer->CreateExplosion(ship.Position());
    ship.Destroy();
  }

  play_events::set(play_events::event_type::explosion, true);
  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  play_events::increment(play_events::counter_type::power_ups_collected);
  powerUp.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& ship, enemy_type_1& enemy) -> void
{
  if( m_levelContainer->PlayerInvulnerable() )
  {
    m_levelContainer->CreateExplosion(enemy.Position());
  }
  else
  {
    m_levelContainer->CreateExplosion(ship.Position());
    ship.Destroy();
  }

  play_events::set(play_events::event_type::explosion, true);
  enemy.Destroy();
}
