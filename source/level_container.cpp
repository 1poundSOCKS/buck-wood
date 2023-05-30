#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"

auto update_all(std::ranges::input_range auto&& objects, int64_t ticks)
{
  for( auto& object : objects )
  {
    object.Update(ticks);
  }
}

auto erase_destroyed(std::ranges::input_range auto&& objects)
{
  auto object = std::begin(objects);

  while( object != std::end(objects) )
  {
    object = object->Destroyed() ? objects.erase(object) : ++object;
  }
}

level_container::level_container()
{
}

level_container::level_container(ID2D1RenderTarget* renderTarget)
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
  return m_activatedTargetCount == m_targetCount;
}

[[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return HasTimedOut() || PlayerDied() || IsComplete();
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  erase_destroyed(m_bullets);

  m_playerShot = false;
  m_targetActivated = false;

  auto playerPosition = m_playerShip.Position();
  auto playerToMouseAngle = CalculateAngle(playerPosition.x, playerPosition.y, inputData.GetMouseData().x, inputData.GetMouseData().y);
  m_playerShip.SetAngle(playerToMouseAngle);

  m_playerShip.SetThrusterOn(inputData.GetMouseData().rightButtonDown);
  m_playerShip.Update(ticks);

  update_all(m_bullets, ticks);

  auto triggerPressed = inputData.GetMouseData().leftButtonDown;

  if( triggerPressed && m_playerShip.CanShoot() )
  {
    m_bullets.emplace_back( bullet { playerPosition.x, playerPosition.y, m_playerShip.Angle() } );
  }

  m_background.SetCentre(playerPosition.x, playerPosition.y);
  m_background.Update(ticks);

  m_asteroids.clear();

  CreateAsteroids(viewRect, std::back_inserter(m_asteroids));

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
}

auto level_container::Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
{
  m_background.Render(viewRect);
  renderer::render_all(m_targets);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_bullets);
  renderer::render(m_playerShip);
}

[[nodiscard]] auto level_container::Targets() const -> const target_collection&
{
  return m_targets;
}

[[nodiscard]] auto level_container::PlayerX() const -> float
{
  return m_playerShip.Position().x;
}

[[nodiscard]] auto level_container::PlayerY() const -> float
{
  return m_playerShip.Position().y;
}

[[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return m_playerHasThrusterOn;
}

[[nodiscard]] auto level_container::PlayerShot() const -> bool
{
  return m_playerShot;
}

[[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerDied;
}

[[nodiscard]] auto level_container::TargetActivated() const -> bool
{
  return m_targetActivated;
}

[[nodiscard]] auto level_container::TicksRemaining() const -> int64_t
{
  return m_ticksRemaining;
}
