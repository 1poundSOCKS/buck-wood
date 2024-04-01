#include "pch.h"
#include "level_collision_handler.h"

auto level_collision_handler::operator()(default_object& object1, default_object& object2) -> void
{
  OnCollision<player_bullet, enemy_bullet_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_2>(object1, object2);
  OnCollision<player_ship, enemy_bullet_1>(object1, object2);
  OnCollision<player_ship, enemy_type_2>(object1, object2);
  OnCollision<player_ship, power_up>(object1, object2);
  OnCollision<player_ship, enemy_type_1>(object1, object2);
  OnCollision<player_bullet, enemy_type_1>(object1, object2);
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
  m_playEvents->Set(play_events::event_type::explosion, true);
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
  m_playEvents->Set(play_events::event_type::explosion, true);
  playerBullet.Destroy();
  enemyBullet.Destroy();
  m_playEvents->Increment(play_events::counter_type::bullets_destroyed);
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  m_playEvents->Set(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
  if( enemy.Destroyed() ) m_playEvents->Increment(play_events::counter_type::enemies_destroyed);
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
    m_playEvents->Set(play_events::event_type::explosion, true);
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

  m_playEvents->Set(play_events::event_type::explosion, true);
  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  m_playEvents->Set(play_events::event_type::power_up_collected, true);
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

  m_playEvents->Set(play_events::event_type::explosion, true);
  enemy.Destroy();
}

auto level_collision_handler::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  m_levelContainer->CreateExplosion(enemy.Position());
  m_playEvents->Set(play_events::event_type::explosion, true);
  bullet.Destroy();
  enemy.Destroy();
}
