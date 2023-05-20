#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "level_asteroid.h"
#include "explosion.h"
#include "game_level_object_generator.h"

level_container::level_container()
{
}

level_container::level_container(ID2D1RenderTarget* renderTarget)
{
  Initialize(renderTarget);
}

auto level_container::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_background.Initialize(renderTarget);
  m_staticObjects.Initialize();
  m_objectContainer.Initialize(renderTarget);
}

auto level_container::AddPlayer(player_ship playerShip) -> void
{
  playerShip.SetPositionUpdate([this](float x, float y, bool thrusterOn) -> void
  {
    m_playerX = x;
    m_playerY = y;
    m_playerHasThrusterOn = thrusterOn;
  });
  
  playerShip.SetEventShot([this](float x, float y, float angle) -> void
  {
    m_playerShot = true;
    m_objectContainer.AppendActiveObject(bullet { x, y, angle });
  });

  playerShip.SetEventDied([this](float x, float y) -> void
  {
    if( !m_playerDied )
    {
      m_objectContainer.AppendActiveObject( explosion_state { x, y} );
    }

    m_playerDied = true;
    m_playerX = x;
    m_playerY = y;
  });

  m_objectContainer.AppendActiveObject(playerShip);
}

auto level_container::AddTarget(level_target levelTarget) -> void
{
  levelTarget.SetActivated([this]()->void
  {
    m_targetActivated = true;
    ++m_activatedTargetCount;
  });

  m_staticObjects.AddTarget(levelTarget);
  ++m_targetCount;
}

auto level_container::AddAsteroid(level_asteroid asteroid) -> void
{
  m_staticObjects.AddAsteroid(asteroid);
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

auto level_container::Update(const object_input_data& inputData, int64_t ticks) -> void
{
  m_playerShot = false;
  m_targetActivated = false;

  m_background.SetCentre(m_playerX, m_playerY);
  m_background.Update(inputData, ticks);
  m_staticObjects.SetCentre(m_playerX, m_playerY);
  m_staticObjects.Update(inputData, ticks);
  m_objectContainer.Update(inputData, ticks);

  m_staticObjects.DoCollisionsWith(m_objectContainer);

  // for( auto& staticObjectContainer : m_staticObjects.GetObjectContainers() )
  // {
  //   m_objectContainer.DoCollisionsWith(staticObjectContainer);
  // }

  m_objectContainer.ClearDestroyedObjects();

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
}

auto level_container::Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
{
  m_background.Render(viewRect);
  m_staticObjects.Render(viewRect);
  m_objectContainer.Render(viewRect);
}

[[nodiscard]] auto level_container::PlayerX() const -> float
{
  return m_playerX;
}

[[nodiscard]] auto level_container::PlayerY() const -> float
{
  return m_playerY;
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
