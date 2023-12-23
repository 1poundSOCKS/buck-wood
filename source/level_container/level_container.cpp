#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "perlin_simplex_noise.h"
#include "game_clock.h"
#include "renderers.h"

auto level_container::Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> update_events
{
  auto interval = game_clock::getInterval(ticks);

  player_ship::update_events playerUpdateEvents;

  // std::optional<game_point> playerPosition = m_playerShip->Destroyed() ? std::nullopt : std::optional<game_point>(m_playerShip->Position());

  if( m_playerShip->Destroyed() )
  {
    m_mines.Update(interval);
  }
  else
  {
    UpdatePlayer(input, interval, &playerUpdateEvents);
    const auto& playerPosition = m_playerShip->Position();

    for( auto& target : m_targets )
    {
      auto targetPosition = target->Position();
      
      if( target->ShootAt(playerPosition) )
      {
        m_mines.Create(level_geometries::MineGeometry(), targetPosition.x, targetPosition.y);
      }
    }

    m_mines.Update(interval, playerPosition.x, playerPosition.y);
  }

  m_targets.Update(interval);
  m_ductFans.Update(interval);
  m_bullets.Update(interval);
  m_explosionParticles.Update(interval);
  m_impactParticles.Update(interval);
  m_thrustParticles.Update(interval);

  DoCollisions();

  m_bullets.EraseDestroyed();
  m_explosionParticles.EraseDestroyed();
  m_impactParticles.EraseDestroyed();
  m_thrustParticles.EraseDestroyed();
  m_mines.EraseDestroyed();

  if( playerUpdateEvents.shot )
  {
    m_bullets.Create(m_playerShip->Position(), m_playerShip->Velocity(), *input.ShootAngle());
  }

  return update_events { playerUpdateEvents.shot, m_collisionChecks.TargetActivationCount() ? true : false, 
    m_collisionChecks.Explosions().size() || m_containmentChecks.Explosions().size() ? true : false };
}

auto level_container::UpdatePlayer(const level_input& input, float interval, player_ship::update_events* updateEvents) -> void
{
  m_playerShip.Update(interval, input.Thrust(), input.Angle(), input.Rotation(), input.ShootAngle() ? true : false, updateEvents);

  if( m_playerShip->ThrusterOn() && m_thrustEmmisionTimer.Update(interval) )
  {
    auto thrustPosition = m_playerShip->RelativePosition(180, 0, -15);
    auto thrustAngle = m_playerShip->Angle() + 180;
    auto thrustVelocity = m_playerShip->RelativeVelocity(thrustAngle, 100);
    m_thrustParticles.Create(thrustPosition, thrustVelocity, 0.3f);
  }
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  renderer::render_all(m_blankObjects);
  renderer::render_all(m_explosionParticles);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_mines);
  renderer::render_all(m_targets);
  renderer::render_all(m_ductFans);
  renderer::render_all(m_impactParticles);
  renderer::render_all(m_bullets);

  if( !m_playerShip->Destroyed() )
  {
    renderer::render(m_playerShip);
  }

  renderer::render_all(m_thrustParticles);
}

auto level_container::DoCollisions() -> void
{
  m_collisionChecks.Reset();
  m_containmentChecks.Reset();

  if( !m_playerShip->Destroyed() )
  {
    if( m_blankObjects.size() )
    {
      auto& border = m_blankObjects.front();
      m_containmentChecks.shipContainment(border.Geometry().Get(), m_playerShip);
    }

    m_collisionChecks.shipToAsteroidCollision(m_playerShip, m_asteroids);
    m_collisionChecks.shipToTargetCollision(m_playerShip, m_targets);
    m_collisionChecks.shipToDuctFanCollision(m_playerShip, m_ductFans);
    m_collisionChecks.shipToMineCollision(m_playerShip, m_mines);
    m_collisionChecks.shipToExplosionCollision(m_playerShip, m_explosionParticles);
  }

  if( m_blankObjects.size() )
  {
    const auto& border = m_blankObjects.front();
    m_containmentChecks.mineContainment(border.Geometry().Get(), m_mines);
    m_containmentChecks.explosionContainment(border.Geometry().Get(), m_explosionParticles);
    m_containmentChecks.thrustContainment(border.Geometry().Get(), m_thrustParticles);
    m_containmentChecks.bulletContainment(border.Geometry().Get(), m_bullets);
  }

  m_collisionChecks.mineToAsteroidCollision(m_mines, m_asteroids);
  m_collisionChecks.mineToDuctFanCollision(m_mines, m_ductFans);
  m_collisionChecks.mineToBulletCollision(m_mines, m_bullets);
  m_collisionChecks.asteroidToBulletCollision(m_asteroids, m_bullets);
  m_collisionChecks.asteroidToExplosionCollision(m_asteroids, m_explosionParticles);
  m_collisionChecks.asteroidToThrustCollision(m_asteroids, m_thrustParticles);
  m_collisionChecks.ductFanToBulletCollision(m_ductFans, m_bullets);
  m_collisionChecks.ductFanToExplosionCollision(m_ductFans, m_explosionParticles);
  m_collisionChecks.ductFanToThrustCollision(m_ductFans, m_thrustParticles);
  m_collisionChecks.targetToBulletCollision(m_targets, m_bullets);

  for( const auto& position : m_containmentChecks.Explosions() )
  {
    m_explosionParticles.Create( level_explosion { position } );
  }

  for( const auto& position : m_collisionChecks.Explosions() )
  {
    m_explosionParticles.Create( level_explosion { position } );
  }

  for( const auto& position : m_containmentChecks.Impacts() )
  {
    m_impactParticles.Create(position);
  }

  for( const auto& position : m_collisionChecks.Impacts() )
  {
    m_impactParticles.Create(position);
  }
}
