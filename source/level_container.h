#pragma once

#include "active_object_container.h"
#include "passive_object_container.h"
#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "level_background.h"
#include "active_object_container.h"
#include "static_objects.h"

class level_container
{
public:

  level_container();
  level_container(ID2D1RenderTarget* renderTarget);
  level_container(const level_container& levelContainer) = delete;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  auto AddPlayer(player_ship playerShip) -> void;
  auto AddTarget(level_target levelTarget) -> void;

  auto Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto PlayerX() const -> float;
  [[nodiscard]] auto PlayerY() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerShot() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto TargetActivated() const -> bool;
  [[nodiscard]] auto TicksRemaining() const -> int64_t;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;

private:

  using active_object_container_type = active_object_container<collision_data, collision_effect>;

  level_background m_background;
  static_objects m_staticObjects;
  active_object_container_type m_objectContainer;

  int64_t m_ticksRemaining = 0;

  float m_playerX = 0;
  float m_playerY = 0;
  bool m_playerHasThrusterOn = false;
  bool m_playerShot = false;
  bool m_playerDied = false;
  bool m_targetActivated = false;
  int m_targetCount = 0;
  int m_activatedTargetCount = 0;
};
