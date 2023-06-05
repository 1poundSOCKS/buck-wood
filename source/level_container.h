#pragma once

#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "explosion.h"
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

  [[nodiscard]] static auto CellWidth() -> int;
  [[nodiscard]] static auto CellHeight() -> int;

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

  inline static const int m_cellWidth { 400 };
  inline static const int m_cellHeight { 400 };

  player_ship_collection m_playerShips;
  bullet_collection m_bullets;
  target_collection m_targets;
  asteroid_container m_asteroids;
  explosion_collection m_explosions;

  int64_t m_ticksRemaining = 0;

  bool m_playerHasThrusterOn = false;
  int m_activatedTargetCount = 0;
};

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.emplace_back( level_target { static_cast<float>(position.x), static_cast<float>(position.y) } );
  });
}
