#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object_functions.h"
#include "level_star.h"
#include "perlin_simplex_noise.h"
#include "level_explosion.h"
#include "game_clock.h"

[[nodiscard]] auto level_container::IsComplete() const -> bool
{
  return m_activatedTargetCount == m_targets.size();
}

[[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return PlayerDied() || IsComplete();
}

auto level_container::Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  auto interval = game_clock::getInterval(ticks);

  m_updateEvents.reset();

  if( m_playerShip.Destroyed() )
  {
    update_all(m_mines, interval);
  }
  else
  {
    UpdatePlayer(input, interval);

    if( m_reloadTimer.Update(interval) && input.ShootAngle() )
    {
      m_bullets.emplace_back( bullet { m_playerShip.Position(), m_playerShip.Velocity(), *input.ShootAngle() } );
      m_updateEvents.playerShot = true;
    }

    const auto& playerPosition = m_playerShip.Position();

    for( auto& target : m_targets )
    {
      auto targetPosition = target.Position();
      
      if( target.ShootAt(playerPosition) )
      {
        m_mines.emplace_back( mine { targetPosition.x, targetPosition.y } );
      }
    }

    for( auto& mine : m_mines )
    {
      mine.Update(interval, playerPosition.x, playerPosition.y);
    }
  }
  
  update_all(m_targets, interval);
  update_all(m_bullets, interval);
  update_all(m_ductFans, interval);
  update_all(m_explosionParticles, interval);
  update_all(m_impactParticles, interval);
  update_all(m_thrustParticles, interval);

  DoCollisions();

  erase_destroyed(m_mines);
  erase_destroyed(m_bullets);
  erase_destroyed(m_explosionParticles);
  erase_destroyed(m_impactParticles);
  erase_destroyed(m_thrustParticles);
}

auto level_container::UpdatePlayer(const level_input& input, float interval) -> void
{
  if( input.Angle() )
  {
    m_playerShip.SetAngle(*input.Angle());
  }

  if( input.Rotation() )
  {
    m_playerShip.Rotate(*input.Rotation() * interval * 20.0f);
  }

  m_playerShip.SetThrust(input.Thrust());

  if( m_playerShip.ThrusterOn() && m_thrustEmmisionTimer.Update(interval) )
  {
    auto thrustPosition = m_playerShip.RelativePosition(180, 0, -10);
    auto thrustAngle = m_playerShip.Angle() + 180;
    game_velocity thrustVelocity { thrustAngle, 100 };
    m_thrustParticles.emplace_back(thrustPosition, thrustVelocity, 0.5f);
  }

  m_playerShip.Update(interval);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  renderer::render_all(m_blankObjects);
  renderer::render_all(m_solidObjects);
  renderer::render_all(m_explosionParticles);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_targets);
  renderer::render_all(m_mines);
  renderer::render_all(m_ductFans);
  renderer::render_all(m_impactParticles);
  renderer::render_all(m_bullets);

  if( !m_playerShip.Destroyed() )
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
  return m_playerShip.Position();
}

[[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return m_playerShip.ThrusterOn();
}

[[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerShip.Destroyed();
}

auto level_container::DoCollisions() -> void
{
  if( !m_playerShip.Destroyed() )
  {
    DoPlayerShipCollisions();
  }

  DoMineCollisions();
  DoBulletCollisions();
  DoExplosionParticleCollisions();
  DoThrustParticleCollisions();

  if( m_blankObjects.size() )
  {
    DoBorderCollisions(m_blankObjects.front());
  }

  do_geometries_to_points_collisions(m_mines, m_bullets, [this](auto& mine, auto& bullet)
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
  if( m_blankObjects.size() && !is_geometry_contained(m_playerShip, m_blankObjects.front()) )
  {
    auto position = m_playerShip.PreviousPosition();
    CreateExplosion(position);
    m_playerShip.ApplyFatalDamage();
  }

  do_geometry_to_geometries_collisions(m_playerShip, m_asteroids, [this](auto& playerShip, auto& asteroid)
  {
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
    playerShip.ApplyFatalDamage();
  });

  do_geometry_to_geometries_collisions(m_playerShip, m_solidObjects, [this](auto& playerShip, auto& solidObject)
  {
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
    playerShip.ApplyFatalDamage();
  });

  do_geometry_to_geometries_collisions(m_playerShip, m_mines, [this](auto& playerShip, auto& mine)
  {
    playerShip.ApplyDamage(2);
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  do_geometry_to_geometries_collisions(m_playerShip, m_targets, [this](auto& playerShip, auto& target)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
  });

  do_geometry_to_geometries_collisions(m_playerShip, m_ductFans, [this](auto& playerShip, auto& ductFan)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
  });
}

auto level_container::DoMineCollisions() -> void
{
  do_geometries_to_geometries_collisions(m_mines, m_asteroids, [this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  do_geometries_to_geometries_collisions(m_mines, m_solidObjects, [this](auto& mine, auto& solidObject)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  do_geometries_to_geometries_collisions(m_mines, m_ductFans, [this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });
}

auto level_container::DoBulletCollisions() -> void
{
  do_geometries_to_points_collisions(m_asteroids, m_bullets, [this](auto& asteroid, auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_solidObjects, m_bullets, [this](auto& solidObject, auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_ductFans, m_bullets, [this](auto& ductFan, auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_targets, m_bullets, [this](auto& target, auto& bullet)
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
  do_geometries_to_points_collisions(m_asteroids, m_explosionParticles, [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  });

  do_geometries_to_points_collisions(m_solidObjects, m_explosionParticles, [this](auto& solidObject, auto& particle)
  {
    particle.Destroy();
  });

  do_geometries_to_points_collisions(m_ductFans, m_explosionParticles, [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  });
}

auto level_container::DoThrustParticleCollisions() -> void
{
  do_geometries_to_points_collisions(m_asteroids, m_thrustParticles, [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  });

  do_geometries_to_points_collisions(m_solidObjects, m_thrustParticles, [this](auto& solidObject, auto& particle)
  {
    particle.Destroy();
  });

  do_geometries_to_points_collisions(m_ductFans, m_thrustParticles, [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  });
}

auto level_container::DoBorderCollisions(const blank_object& border) -> void
{
  check_geometries_contained(m_mines, border, [this](auto& mine)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
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
