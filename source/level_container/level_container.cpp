#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object_functions.h"
#include "perlin_simplex_noise.h"
#include "level_explosion.h"
#include "game_clock.h"
#include "renderers.h"

[[nodiscard]] auto level_container::IsComplete() const -> bool
{
  return m_activatedTargetCount == m_targets.Size();
}

[[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return PlayerDied() || IsComplete();
}

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

[[nodiscard]] auto level_container::Targets() const -> const target_collection&
{
  return m_targets;
}

[[nodiscard]] auto level_container::PlayerPosition() const -> game_point
{
  return m_playerShip->Position();
}

[[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return m_playerShip->ThrusterOn();
}

[[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerShip->Destroyed();
}

auto level_container::DoCollisions() -> void
{
  if( !m_playerShip->Destroyed() && m_blankObjects.size() )
  {
    const auto& border = m_blankObjects.front();

    if( !is_geometry_contained(m_playerShip, border) )
    {
      auto position = m_playerShip->PreviousPosition();
      CreateExplosion(position);
      m_playerShip->ApplyFatalDamage();
    }
  }

  if( !m_playerShip->Destroyed() )
  {
    DoPlayerShipCollisions();
  }

  if( m_blankObjects.size() )
  {
    DoBorderCollisions(m_blankObjects.front());
  }

  DoMineCollisions();
  DoBulletCollisions();
  DoExplosionParticleCollisions();
  DoThrustParticleCollisions();

  m_mines.DoCollisionsWithPoints(m_bullets, [this](auto& mine, auto& bullet) -> void
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    bullet.Destroy();
    m_updateEvents.mineExploded = true;
  });
}

auto level_container::DoPlayerShipCollisions() -> void
{
  m_asteroids.DoCollisionsWithGeometry(m_playerShip, [this](auto& asteroid) -> void
  {
    auto position = m_playerShip->PreviousPosition();
    CreateExplosion(position);
    m_playerShip->ApplyFatalDamage();
  });

  m_mines.DoCollisionsWithGeometry(m_playerShip, [this](auto& mine) -> void
  {
    m_playerShip->ApplyDamage(2);
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  m_targets.DoCollisionsWithGeometry(m_playerShip, [this](auto& target) -> void
  {
    m_playerShip->ApplyFatalDamage();
    auto position = m_playerShip->PreviousPosition();
    CreateExplosion(position);
  });

  m_ductFans.DoCollisionsWithGeometry(m_playerShip, [this](auto& ductFan) -> void
  {
    m_playerShip->ApplyFatalDamage();
    auto position = m_playerShip->PreviousPosition();
    CreateExplosion(position);
  });
}

auto level_container::DoMineCollisions() -> void
{
  m_mines.DoCollisionsWithGeometries(m_asteroids, [this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  m_mines.DoCollisionsWithGeometries(m_ductFans, [this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });
}

auto level_container::DoBulletCollisions() -> void
{
  m_asteroids.DoCollisionsWithPoints(m_bullets, [this](auto& asteroid, auto& bullet) -> void
  {
    m_impactParticles.emplace_back(bullet.Position());
    bullet.Destroy();
  });

  m_ductFans.DoCollisionsWithPoints(m_bullets, [this](auto& ductFan, auto& bullet) -> void
  {
    m_impactParticles.emplace_back(bullet.Position());
    bullet.Destroy();
  });

  m_targets.DoCollisionsWithPoints(m_bullets, [this](auto& target, auto& bullet) -> void
  {
    m_impactParticles.emplace_back(bullet.Position());

    if( !target.IsActivated() )
    {
      target.HitByBullet();

      if( target.IsActivated() )
      {
        ++m_activatedTargetCount;
        m_updateEvents.targetActivated = true;
      }
    }

    bullet.Destroy();
  });
}

auto level_container::DoExplosionParticleCollisions() -> void
{
  m_playerShip.DoCollisionsWithPoints(m_explosionParticles, [this](auto& particle) -> void
  {
    particle.Destroy();
  });

  m_asteroids.DoCollisionsWithPoints(m_explosionParticles, [this](auto& asteroid, auto& particle) -> void
  {
    particle.Destroy();
  });

  m_ductFans.DoCollisionsWithPoints(m_explosionParticles, [this](auto& ductFan, auto& particle) -> void
  {
    particle.Destroy();
  });
}

auto level_container::DoThrustParticleCollisions() -> void
{
  m_asteroids.DoCollisionsWithPoints(m_thrustParticles, [this](auto& asteroid, auto& particle) -> void
  {
    particle.Destroy();
  });

  m_ductFans.DoCollisionsWithPoints(m_thrustParticles, [this](auto& ductFan, auto& particle) -> void
  {
    particle.Destroy();
  });
}

auto level_container::DoBorderCollisions(const blank_object& border) -> void
{
  check_geometries_contained(m_mines, border, [this](auto& mine)
  {
    auto position = mine.Object().PreviousPosition();
    CreateExplosion(position);
    mine.Object().Destroy();
    m_updateEvents.mineExploded = true;
  });

  check_points_contained(m_explosionParticles, border, [this](auto& particle)
  {
    particle.Destroy();
  });

  check_points_contained(m_thrustParticles, border, [this](auto& particle)
  {
    particle.Destroy();
  });

  check_points_contained(m_bullets, border, [this](auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
    bullet.Destroy();
  });
}

auto level_container::CreateExplosion(const game_point& position) -> void
{
  level_explosion levelExplosion { position };
  std::ranges::copy(levelExplosion, std::back_inserter(m_explosionParticles));
}
