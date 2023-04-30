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
    m_playerDied = false;
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
  });

  m_objectContainer.AppendActiveObject(levelTarget);
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
  m_timeoutTicks = performance_counter::QueryFrequency() * time;
}

auto level_container::HasTimedOut() const -> bool
{
  return m_timeoutTicks == 0;
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks) -> void
{
  m_playerShot = false;
  m_targetActivated = false;

  m_objectContainer.Update(inputData, ticks);
  m_objectContainer.DoCollisions();
  m_objectContainer.ClearDestroyedObjects();

  m_timeoutTicks -= ticks;
  m_timeoutTicks = max(0, m_timeoutTicks);
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
