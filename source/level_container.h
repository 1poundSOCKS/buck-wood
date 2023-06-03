#pragma once

#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "level_background.h"
#include "explosion.h"
#include "game_level_object_generator.h"
#include "renderers.h"
#include "asteroid_container.h"

class level_container
{
public:

  struct events
  {
    bool playerShot { false };
    bool targetActivated { false };
  };

  using events_ptr = std::unique_ptr<events>;

  using player_ship_collection = std::vector<player_ship>;
  using bullet_collection = std::list<bullet>;
  using target_collection = std::vector<level_target>;
  using explosion_collection = std::list<explosion>;

  level_container();
  level_container(const level_container& levelContainer) = delete;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  auto AddTargets(std::ranges::input_range auto&& targets) -> void;

  auto Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> events_ptr;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto TicksRemaining() const -> int64_t;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;

private:

  player_ship_collection m_playerShips;
  bullet_collection m_bullets;
  target_collection m_targets;
  asteroid_container m_asteroids;
  level_background m_background;
  explosion_collection m_explosions;

  int64_t m_ticksRemaining = 0;

  bool m_playerHasThrusterOn = false;
  int m_activatedTargetCount = 0;
};

auto level_container::AddTargets(std::ranges::input_range auto&& targets) -> void
{
  std::ranges::copy(targets, std::back_inserter(m_targets));
}
