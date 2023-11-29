#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object.h"
#include "level_star.h"
#include "perlin_simplex_noise.h"
#include "level_explosion.h"
#include "game_clock.h"

level_container::level_container() : m_reloadTimer { static_cast<float>(m_shotTimeNumerator) / static_cast<float>(m_shotTimeDenominator) }
{
}

auto level_container::SetTimeout(int time) -> void
{
  m_ticksRemaining = performance_counter::QueryFrequency() * time;
}

auto level_container::HasTimedOut() const -> bool
{
  return m_ticksRemaining == 0;
}

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
    for( auto& mine : m_mines )
    {
      mine.Update(interval);
    }
  }
  else
  {
    const auto& playerPosition = m_playerShip.Position();

    if( input.Angle() )
    {
      m_playerShip.SetAngle(*input.Angle());
    }

    if( input.Rotation() )
    {
      m_playerShip.Rotate(*input.Rotation() * interval * 20.0f);
    }

    m_playerShip.SetThrust(input.Thrust());

    m_playerShip.Update(interval);

    auto reloaded = m_reloadTimer.Update(interval);
    auto triggerPressed = input.ShootAngle() ? true : false;

    if( reloaded && triggerPressed )
    {
      m_bullets.emplace_back( bullet { m_playerShip.Position(), m_playerShip.Velocity(), *input.ShootAngle() } );
      m_updateEvents.playerShot = true;
    }

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
  update_all(m_explosionParticles, interval);
  update_all(m_impactParticles, interval);

  // auto grid = GetGrid(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
  
  // m_asteroids.Update(grid);
  // m_solidObjects.Update(grid);

  DoCollisions();

  erase_destroyed(m_mines);
  erase_destroyed(m_bullets);
  erase_destroyed(m_explosionParticles);
  erase_destroyed(m_impactParticles);

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto starGrid = level_grid { 100, 100,  viewRect.left, viewRect.top, viewRect.right, viewRect.bottom };
  
  auto starView = starGrid | std::ranges::views::filter([this](const auto& cell)
  {
    auto inside = m_blankObjects.size() && have_geometry_and_point_collided(m_blankObjects.front(), cell);
    return inside && psn::GetNoise(cell.Position().x, cell.Position().y) > 0.90f;
  })
  | std::ranges::views::transform([](const auto& cell)
  {
    return level_star { cell.Position().x, cell.Position().y };
  });

  renderer::render_all(m_blankObjects);
  renderer::render_all(starView);
  renderer::render_all(m_solidObjects);
  renderer::render_all(m_explosionParticles);
  // renderer::render_all(m_asteroids);
  renderer::render_all(m_targets);
  renderer::render_all(m_mines);
  renderer::render_all(m_impactParticles);
  renderer::render_all(m_bullets);

  if( !m_playerShip.Destroyed() )
  {
    renderer::render(m_playerShip);
  }
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

[[nodiscard]] auto level_container::TicksRemaining() const -> int64_t
{
  return m_ticksRemaining;
}

auto level_container::DoCollisions() -> void
{
  if( !m_playerShip.Destroyed() )
  {
    // do_geometry_to_geometries_collisions(m_playerShip, m_asteroids, [this](auto& playerShip, auto& asteroid)
    // {
    //   auto position = playerShip.PreviousPosition();
    //   CreateExplosion(position);
    //   playerShip.ApplyFatalDamage();
    // });
    if( m_blankObjects.size() && !is_geometry_contained(m_playerShip, m_blankObjects.front()) )
    {
      auto position = m_playerShip.PreviousPosition();
      CreateExplosion(position);
      m_playerShip.ApplyFatalDamage();
    }

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
  }

  // do_geometries_to_geometries_collisions(m_mines, m_asteroids, [this, updateEvents](auto& mine, auto& asteroid)
  // {
  //   auto position = mine.PreviousPosition();
  //   CreateExplosion(position);
  //   mine.Destroy();
  //   updateEvents->mineExploded = true;
  // });

  // do_geometries_to_points_collisions(m_asteroids, m_bullets, [this](auto& asteroid, auto& bullet)
  // {
  //   m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
  //   bullet.Destroy();
  // });

  // do_geometries_to_points_collisions(m_asteroids, m_explosionParticles, [this](auto& asteroid, auto& particle)
  // {
  //   particle.Destroy();
  // });

  do_geometries_to_geometries_collisions(m_mines, m_solidObjects, [this](auto& mine, auto& solidObject)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  });

  if( m_blankObjects.size() )
  {
    check_geometries_contained(m_mines, m_blankObjects.front(), [this](auto& mine)
    {
      auto position = mine.PreviousPosition();
      CreateExplosion(position);
      mine.Destroy();
      m_updateEvents.mineExploded = true;
    });
  }

  do_geometries_to_points_collisions(m_solidObjects, m_bullets, [this](auto& solidObject, auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_solidObjects, m_explosionParticles, [this](auto& solidObject, auto& particle)
  {
    particle.Destroy();
  });

  if( m_blankObjects.size() )
  {
    check_points_contained(m_explosionParticles, m_blankObjects.front(), [this](auto& particle)
    {
      particle.Destroy();
    });
  }

  do_geometries_to_points_collisions(m_targets, m_bullets, [this](auto& target, auto& bullet)
  {
    m_impactParticles.emplace_back( impact_particle { bullet.Position() } );

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

  if( m_blankObjects.size() )
  {
    check_points_contained(m_bullets, m_blankObjects.front(), [this](auto& bullet)
    {
      m_impactParticles.emplace_back( impact_particle { bullet.Position() } );
      bullet.Destroy();
    });
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

auto level_container::CreateExplosion(const game_point& position) -> void
{
  level_explosion levelExplosion { position };
  std::ranges::copy(levelExplosion, std::back_inserter(m_explosionParticles));
}
