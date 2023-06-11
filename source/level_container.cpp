#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object.h"
#include "level_star.h"
#include "perlin_simplex_noise.h"

level_container::level_container()
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

  if( !m_playerShip.Destroyed() )
  {
    UpdatePlayer(inputData, ticks, updateEvents.get());
  }
  
  UpdateTargets(ticks, updateEvents.get());

  update_all(m_bullets, ticks);
  update_all(m_explosions, ticks);

  UpdateMines(ticks);

  auto grid = GetGrid(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
  
  m_asteroids.Update(grid);

  DoCollisions(updateEvents.get());

  erase_destroyed(m_explosions);
  erase_destroyed(m_mines);
  erase_destroyed(m_bullets);

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);

  return updateEvents;
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto starGrid = level_grid { 100, 100,  viewRect.left, viewRect.top, viewRect.right, viewRect.bottom };
  
  auto starView = starGrid | std::ranges::views::filter([](const auto& cell)
  {
    
    return psn::GetNoise(static_cast<float>(cell.x), static_cast<float>(cell.y)) > 0.90f;
  })
  | std::ranges::views::transform([](const auto& cell)
  {
    return level_star { static_cast<float>(cell.x), static_cast<float>(cell.y) };
  });

  renderer::render_all(starView);
  renderer::render_all(m_explosions);
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

auto level_container::UpdatePlayer(const object_input_data& inputData, int64_t ticks, update_events* updateEvents) -> void
{
  auto playerPosition = m_playerShip.Position();
  auto playerToMouseAngle = CalculateAngle(playerPosition.x, playerPosition.y, inputData.GetMouseData().x, inputData.GetMouseData().y);

  m_playerShip.SetAngle(playerToMouseAngle);
  m_playerShip.SetThrusterOn(inputData.GetMouseData().rightButtonDown);

  auto triggerPressed = inputData.GetMouseData().leftButtonDown;

  if( triggerPressed && m_playerShip.CanShoot() )
  {
    m_bullets.emplace_back( bullet { playerPosition.x, playerPosition.y, m_playerShip.Angle() } );
    updateEvents->playerShot = true;
  }

  m_playerShip.Update(ticks);
}

auto level_container::UpdateTargets(int64_t ticks, update_events* updateEvents) -> void
{
  if( !m_playerShip.Destroyed() )
  {
    auto playerPosition = m_playerShip.Position();

    for( auto& target : m_targets )
    {
      auto targetPosition = target.Position();
      
      if( target.ShootAt(playerPosition) )
      {
        m_mines.emplace_back( mine { targetPosition.x, targetPosition.y } );
      }
    }
  }

  update_all(m_targets, ticks);
}

auto level_container::UpdateMines(int64_t ticks) -> void
{
  auto playerPosition = m_playerShip.Position();

  if( !m_playerShip.Destroyed() )
  {
    for( auto& mine : m_mines )
    {
      mine.Update(ticks, playerPosition.x, playerPosition.y);
    }
  }
  else
  {
    for( auto& mine : m_mines )
    {
      mine.Update(ticks);
    }
  }
}

auto level_container::DoCollisions(update_events* updateEvents) -> void
{
  if( !m_playerShip.Destroyed() )
  {
    do_geometry_to_geometries_collisions(m_playerShip, m_asteroids, [this](auto& playerShip, auto& asteroid)
    {
      playerShip.Destroy();
      auto position = playerShip.Position();
      m_explosions.emplace_back( explosion { position.x, position.y } );
    });

    do_geometry_to_geometries_collisions(m_playerShip, m_mines, [this](auto& mine, auto& playerShip)
    {
      mine.Destroy();
      playerShip.Destroy();
      auto position = playerShip.Position();
      m_explosions.emplace_back( explosion { position.x, position.y } );
    });

    do_geometry_to_geometries_collisions(m_playerShip, m_targets, [this](auto& playerShip, auto& target)
    {
      playerShip.Destroy();
      auto position = playerShip.Position();
      m_explosions.emplace_back( explosion { position.x, position.y } );
    });
  }

  do_geometries_to_geometries_collisions(m_mines, m_asteroids, [this](auto& mine, auto& asteroid)
  {
    mine.Destroy();
    auto position = mine.Position();
    m_explosions.emplace_back( explosion { position.x, position.y } );
  });

  do_geometries_to_points_collisions(m_asteroids, m_bullets, [](auto& asteroid, auto& bullet)
  {
    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_targets, m_bullets, [this, updateEvents](auto& target, auto& bullet)
  {
    if( !target.IsActivated() )
    {
      target.Activate();
      ++m_activatedTargetCount;
      updateEvents->targetActivated = true;
    }

    bullet.Destroy();
  });

  do_geometries_to_points_collisions(m_mines, m_bullets, [this](auto& mine, auto& bullet)
  {
    auto position = mine.Position();
    m_explosions.emplace_back( explosion { position.x, position.y } );
    mine.Destroy();
    bullet.Destroy();
  });
}
