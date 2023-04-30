#pragma once

#include "active_object_container.h"
#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"

class level_container
{
public:

  level_container();
  level_container(const level_container& levelContainer) = delete;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  auto AddPlayer(player_ship playerShip) -> void;
  auto AddTarget(level_target levelTarget) -> void;

  [[nodiscard]] auto GetObjectContainer() const -> const active_object_container&;
  [[nodiscard]] auto GetObjectContainer() -> active_object_container&;

  auto Update(const object_input_data& inputData, int64_t ticks) -> void;

  [[nodiscard]] auto PlayerX() const -> float;
  [[nodiscard]] auto PlayerY() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerShot() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto TargetActivated() const -> bool;  

private:

  active_object_container m_objectContainer;

  int64_t m_timeoutTicks = 0;

  float m_playerX = 0;
  float m_playerY = 0;
  bool m_playerHasThrusterOn = false;
  bool m_playerShot = false;
  bool m_playerDied = false;
  bool m_targetActivated = false;
};
