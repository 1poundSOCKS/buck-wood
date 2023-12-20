#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object_functions.h"
#include "perlin_simplex_noise.h"
#include "level_explosion.h"
#include "game_clock.h"
#include "renderers.h"

auto level_container::Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  auto interval = game_clock::getInterval(ticks);

  m_updateEvents.reset();

  if( m_playerShip->Destroyed() )
  {
    m_mines.Update(interval);
  }
  else
  {
    UpdatePlayer(input, interval);

    if( m_reloadTimer.Update(interval) && input.ShootAngle() )
    {
      m_bullets.emplace_back(m_playerShip->Position(), m_playerShip->Velocity(), *input.ShootAngle());
      m_updateEvents.playerShot = true;
    }

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
  
  update_all(m_bullets, interval);
  update_all(m_explosionParticles, interval);
  update_all(m_impactParticles, interval);
  update_all(m_thrustParticles, interval);

  DoCollisions();

  erase_destroyed(m_bullets);
  erase_destroyed(m_explosionParticles);
  erase_destroyed(m_impactParticles);
  erase_destroyed(m_thrustParticles);

  m_mines.EraseDestroyed();
}

auto level_container::UpdatePlayer(const level_input& input, float interval) -> void
{
  m_playerShip.Update(interval, input.Thrust(), input.Angle(), input.Rotation());

  if( m_playerShip->ThrusterOn() && m_thrustEmmisionTimer.Update(interval) )
  {
    auto thrustPosition = m_playerShip->RelativePosition(180, 0, -15);
    auto thrustAngle = m_playerShip->Angle() + 180;
    auto thrustVelocity = m_playerShip->RelativeVelocity(thrustAngle, 100);
    m_thrustParticles.emplace_back(thrustPosition, thrustVelocity, 0.3f);
  }
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  renderer::render_all(m_blankObjects);
  renderer::render_all(m_explosionParticles);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_targets);
  renderer::render_all(m_mines);
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
  if( !m_playerShip->Destroyed() )
  {
    if( m_blankObjects.size() )
    {
      auto& border = m_blankObjects.front();
      m_shipContainment(border.Geometry().Get(), m_playerShip);
    }

    m_shipToAsteroidCollision(m_playerShip, m_asteroids);
    m_shipToTargetCollision(m_playerShip, m_targets);
    m_shipToDuctFanCollision(m_playerShip, m_ductFans);
    m_shipToMineCollision(m_playerShip, m_mines);
    m_shipToExplosionCollision(m_playerShip, m_explosionParticles);
  }

  if( m_blankObjects.size() )
  {
    const auto& border = m_blankObjects.front();
    m_mineContainment(border.Geometry().Get(), m_mines);
    m_explosionContainment(border.Geometry().Get(), m_explosionParticles);
    m_thrustContainment(border.Geometry().Get(), m_thrustParticles);
    m_bulletContainment(border.Geometry().Get(), m_bullets);
  }

  m_mineToAsteroidCollision(m_mines, m_asteroids);
  m_mineToDuctFanCollision(m_mines, m_ductFans);
  m_mineToBulletCollision(m_mines, m_bullets);

  m_asteroidToBulletCollision(m_asteroids, m_bullets);
  m_asteroidToExplosionCollision(m_asteroids, m_explosionParticles);
  m_asteroidToThrustCollision(m_asteroids, m_thrustParticles);

  m_ductFanToBulletCollision(m_ductFans, m_bullets);
  m_ductFanToExplosionCollision(m_ductFans, m_explosionParticles);
  m_ductFanToThrustCollision(m_ductFans, m_thrustParticles);

  m_targetToBulletCollision(m_targets, m_bullets);
}

auto level_container::CreateExplosion(const game_point& position) -> void
{
  level_explosion levelExplosion { position };
  std::ranges::copy(levelExplosion, std::back_inserter(m_explosionParticles));
}
