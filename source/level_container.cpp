#include "pch.h"
#include "level_container.h"
#include "bullet.h"

level_container::level_container()
{
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

  m_objectContainer.AppendActiveObject(levelTarget);
  ++m_targetCount;
}

[[nodiscard]] auto level_container::GetObjectContainer() const -> const active_object_container&
{
  return m_objectContainer;
}

[[nodiscard]] auto level_container::GetObjectContainer() -> active_object_container&
{
  return m_objectContainer;
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

auto level_container::Update(const object_input_data& inputData, int64_t ticks) -> void
{
  m_playerShot = false;
  m_targetActivated = false;

  m_objectContainer.Update(inputData, ticks);
  m_objectContainer.DoCollisions();
  m_objectContainer.ClearDestroyedObjects();

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
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
