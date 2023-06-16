#pragma once

#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "explosion.h"
#include "renderers.h"
#include "asteroid_container.h"
#include "mine.h"

class level_container
{
public:

  struct update_events
  {
    bool playerShot { false };
    bool targetActivated { false };
    bool mineExploded { false };
  };

  using update_events_ptr = std::unique_ptr<update_events>;

  using bullet_collection = std::list<bullet>;
  using target_collection = std::vector<level_target>;
  using mine_collection = std::vector<mine>;
  using explosion_collection = std::list<explosion>;

  level_container();
  level_container(const level_container& levelContainer) = delete;

  [[nodiscard]] static auto CellWidth() -> int;
  [[nodiscard]] static auto CellHeight() -> int;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  [[nodiscard]] auto GetGrid(float left, float top, float right, float bottom) -> level_grid;

  auto AddTargets(std::ranges::input_range auto&& targets) -> void;
  auto AddMines(std::ranges::input_range auto&& mines) -> void;

  auto Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> update_events_ptr;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto TicksRemaining() const -> int64_t;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;

private:

  auto UpdatePlayer(const object_input_data& inputData, int64_t ticks, update_events* updateEvents) -> void;
  auto UpdateTargets(int64_t ticks, update_events* updateEvents) -> void;
  auto UpdateMines(int64_t ticks) -> void;
  auto DoCollisions(update_events* updateEvents) -> void;

  inline static const int m_cellWidth { 800 };
  inline static const int m_cellHeight { 800 };

  player_ship m_playerShip;

  bullet_collection m_bullets;
  target_collection m_targets;
  mine_collection m_mines;
  asteroid_container m_asteroids;
  explosion_collection m_explosions;

  int64_t m_ticksRemaining = 0;

  bool m_playerHasThrusterOn = false;
  int m_activatedTargetCount = 0;
};

auto level_container::AddTargets(std::ranges::input_range auto&& cells) -> void
{
  std::ranges::for_each(cells, [this](const auto& cell)
  {
    m_targets.emplace_back( level_target { cell.Position().x, cell.Position().y } );
  });
}

auto level_container::AddMines(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_mines.emplace_back( mine { static_cast<float>(position.x), static_cast<float>(position.y) } );
  });
}
