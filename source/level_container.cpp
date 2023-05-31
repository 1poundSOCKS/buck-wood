#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"
#include "level_object.h"

level_container::level_container()
{
  m_playerShips.emplace_back( player_ship {} );
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
  return HasTimedOut() || PlayerDied() || IsComplete();
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  m_playerShot = false;
  m_targetActivated = false;

  for( auto& playerShip : m_playerShips )
  {
    auto playerPosition = playerShip.Position();
    auto playerToMouseAngle = CalculateAngle(playerPosition.x, playerPosition.y, inputData.GetMouseData().x, inputData.GetMouseData().y);
    playerShip.SetAngle(playerToMouseAngle);
    playerShip.SetThrusterOn(inputData.GetMouseData().rightButtonDown);
  }

  update_all(m_playerShips, ticks);
  update_all(m_bullets, ticks);
  update_all(m_explosions, ticks);

  auto triggerPressed = inputData.GetMouseData().leftButtonDown;

  for( auto& playerShip : m_playerShips )
  {
    auto playerPosition = playerShip.Position();
    
    if( triggerPressed && playerShip.CanShoot() )
    {
      m_bullets.emplace_back( bullet { playerPosition.x, playerPosition.y, playerShip.Angle() } );
    }

    m_background.SetCentre(playerPosition.x, playerPosition.y);
  }

  m_background.Update(ticks);

  m_asteroids.clear();

  CreateAsteroids(viewRect, std::back_inserter(m_asteroids));

  do_geometry_to_geometry_collisions(m_asteroids, m_playerShips, [this](auto& asteroid, auto& playerShip)
  {
    playerShip.Destroy();
    auto position = playerShip.Position();
    m_explosions.emplace_back( explosion { position.x, position.y } );
  });

  do_geometry_to_geometry_collisions(m_targets, m_playerShips, [this](auto& target, auto& playerShip)
  {
    playerShip.Destroy();
    auto position = playerShip.Position();
    m_explosions.emplace_back( explosion { position.x, position.y } );
  });

  do_geometry_to_point_collisions(m_asteroids, m_bullets, [](auto& asteroid, auto& bullet)
  {
    bullet.Destroy();
  });

  do_geometry_to_point_collisions(m_targets, m_bullets, [this](auto& target, auto& bullet)
  {
    if( !target.IsActivated() )
    {
      target.Activate();
      ++m_activatedTargetCount;
    }

    bullet.Destroy();
  });

  erase_destroyed(m_playerShips);
  erase_destroyed(m_explosions);
  erase_destroyed(m_bullets);

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
}

auto level_container::Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
{
  m_background.Render(viewRect);
  renderer::render_all(m_explosions);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_targets);
  renderer::render_all(m_bullets);
  renderer::render_all(m_playerShips);
}

[[nodiscard]] auto level_container::Targets() const -> const target_collection&
{
  return m_targets;
}

[[nodiscard]] auto level_container::PlayerPosition() const -> game_point
{
  return std::reduce(std::cbegin(m_playerShips), std::end(m_playerShips), game_point { 0, 0 }, [](auto postion, const auto& playerShip) { return playerShip.Position(); });
}

[[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return std::reduce(std::cbegin(m_playerShips), std::end(m_playerShips), false, [](auto thrusterOn, const auto& playerShip) { return playerShip.ThrusterOn(); });
}

[[nodiscard]] auto level_container::PlayerShot() const -> bool
{
  return m_playerShot;
}

[[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerShips.size() == 0;
}

[[nodiscard]] auto level_container::TargetActivated() const -> bool
{
  return m_targetActivated;
}

[[nodiscard]] auto level_container::TicksRemaining() const -> int64_t
{
  return m_ticksRemaining;
}
