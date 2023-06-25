#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object.h"
#include "level_star.h"
#include "perlin_simplex_noise.h"
#include "level_explosion.h"

level_container::level_container() : m_reloadTimer { static_cast<float>(m_shotTimeNumerator) / static_cast<float>(m_shotTimeDenominator) }
{
}

auto level_container::SetTimeout(int time) -> void
{
  m_ticksRemaining = performance_counter::QueryFrequency() * time;
}

[[nodiscard]] auto level_container::CellWidth() -> int
{
  return m_cellWidth;
}

[[nodiscard]] auto level_container::CellHeight() -> int
{
  return m_cellHeight;
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

[[nodiscard]] auto level_container::GetGrid(float left, float top, float right, float bottom) -> level_grid
{
  return { m_cellWidth, m_cellHeight, left, top, right, bottom };
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> update_events_ptr
{
  auto updateEvents = std::make_unique<update_events>();

  auto interval = framework::gameUpdateInterval(ticks);

  if( m_playerShip.Destroyed() )
  {
    for( auto& mine : m_mines )
    {
      mine.Update(ticks);
    }
  }
  else
  {
    const auto& playerPosition = m_playerShip.Position();

    auto playerToMouseAngle = CalculateAngle(playerPosition.x, playerPosition.y, inputData.GetMouseData().x, inputData.GetMouseData().y);

    m_playerShip.SetAngle(playerToMouseAngle);
    m_playerShip.SetThrusterOn(inputData.GetMouseData().rightButtonDown);

    m_playerShip.Update(interval);

    auto reloaded = m_reloadTimer.Update(interval);
    auto triggerPressed = inputData.GetMouseData().leftButtonDown;

    if( reloaded && triggerPressed )
    {
      m_bullets.emplace_back( bullet { m_playerShip.Position(), m_playerShip.Velocity(), m_playerShip.Angle() } );
      updateEvents->playerShot = true;
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
      mine.Update(ticks, playerPosition.x, playerPosition.y);
    }
  }
  
  update_all(m_targets, interval);
  update_all(m_bullets, interval);
  update_all(m_explosionParticles, interval);

  auto grid = GetGrid(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
  
  m_asteroids.Update(grid);

  DoCollisions(updateEvents.get());

  erase_destroyed(m_mines);
  erase_destroyed(m_bullets);
  erase_destroyed(m_explosionParticles);

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);

  return updateEvents;
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto starGrid = level_grid { 100, 100,  viewRect.left, viewRect.top, viewRect.right, viewRect.bottom };
  
  auto starView = starGrid | std::ranges::views::filter([](const auto& cell)
  {
    return psn::GetNoise(cell.Position().x, cell.Position().y) > 0.90f;
  })
  | std::ranges::views::transform([](const auto& cell)
  {
    return level_star { cell.Position().x, cell.Position().y };
  });

  renderer::render_all(starView);
  renderer::render_all(m_explosionParticles);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_targets);
  renderer::render_all(m_mines);
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

auto level_container::DoCollisions(update_events* updateEvents) -> void
{
  if( !m_playerShip.Destroyed() )
  {
    do_geometry_to_geometries_collisions(m_playerShip, m_asteroids, [this](auto& playerShip, auto& asteroid)
    {
      playerShip.Destroy();
      auto position = playerShip.Position();
      CreateExplosion(position);
    });

    do_geometry_to_geometries_collisions(m_playerShip, m_mines, [this, updateEvents](auto& mine, auto& playerShip)
    {
      mine.Destroy();
      playerShip.Destroy();
      auto position = playerShip.Position();
      CreateExplosion(position);
      updateEvents->mineExploded = true;
    });

    do_geometry_to_geometries_collisions(m_playerShip, m_targets, [this](auto& playerShip, auto& target)
    {
      playerShip.Destroy();
      auto position = playerShip.Position();
      CreateExplosion(position);
    });
  }

  do_geometries_to_geometries_collisions(m_mines, m_asteroids, [this, updateEvents](auto& mine, auto& asteroid)
  {
    mine.Destroy();
    auto position = mine.Position();
    CreateExplosion(position);
    updateEvents->mineExploded = true;
  });

  do_geometries_to_points_collisions(m_asteroids, m_bullets, [](auto& asteroid, auto& bullet)
  {
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_targets, m_bullets, [this, updateEvents](auto& target, auto& bullet)
  {
    if( !target.IsActivated() )
    {
      target.HitByBullet();

      if( target.IsActivated() )
      {
        ++m_activatedTargetCount;
        updateEvents->targetActivated = true;
      }
    }

    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_mines, m_bullets, [this, updateEvents](auto& mine, auto& bullet)
  {
    auto position = mine.Position();
    CreateExplosion(position);
    mine.Destroy();
    bullet.Destroy();
    updateEvents->mineExploded = true;
  });
}

auto level_container::CreateExplosion(const game_point& position) -> void
{
  level_explosion levelExplosion { position };
  std::ranges::copy(levelExplosion, std::back_inserter(m_explosionParticles));
}
