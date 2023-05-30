#pragma once

#include "active_object_container.h"
#include "passive_object_container.h"
#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "level_background.h"
#include "active_object_container.h"
#include "game_level_object_generator.h"
#include "renderers.h"

class level_container
{
public:

  using bullet_collection = std::list<bullet>;
  using target_collection = std::vector<level_target>;
  using asteroid_collection = std::list<level_asteroid>;

  level_container();
  level_container(ID2D1RenderTarget* renderTarget);
  level_container(const level_container& levelContainer) = delete;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  auto AddTargets(std::ranges::input_range auto&& targets) -> void;

  auto Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
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

  using active_object_container_type = active_object_container<collision_data, collision_effect, renderer>;

  auto CreateAsteroids(D2D1_RECT_F viewRect, auto inserter) -> void;

  player_ship m_playerShip;
  bullet_collection m_bullets;
  target_collection m_targets;
  asteroid_collection m_asteroids;
  level_background m_background;

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

auto level_container::AddTargets(std::ranges::input_range auto&& targets) -> void
{
  std::ranges::copy(targets, std::back_inserter(m_targets));
}

auto level_container::CreateAsteroids(D2D1_RECT_F viewRect, auto inserter) -> void
{
  auto smallColumnWidth = 100;
  auto smallRowHeight = 100;

  auto smallLeftColumn = static_cast<int>(viewRect.left) / smallColumnWidth - 1;
  auto smallRightColumn = static_cast<int>(viewRect.right) / smallColumnWidth + 1;
  auto smallTopRow = static_cast<int>(viewRect.top) / smallRowHeight - 1;
  auto smallBottomRow = static_cast<int>(viewRect.bottom) / smallRowHeight + 1;

  game_level_object_generator smallAsteroidGenerator(smallLeftColumn, smallRightColumn, smallColumnWidth, smallTopRow, smallBottomRow, smallRowHeight, 0.85f, 0.89f, 13.0f);
  smallAsteroidGenerator.InsertInto(inserter);

  auto largeColumnWidth = 400;
  auto largeRowHeight = 400;

  auto largeLeftColumn = static_cast<int>(viewRect.left) / largeColumnWidth - 1;
  auto largeRightColumn = static_cast<int>(viewRect.right) / largeColumnWidth + 1;
  auto largeTopRow = static_cast<int>(viewRect.top) / largeRowHeight - 1;
  auto largeBottomRow = static_cast<int>(viewRect.bottom) / largeRowHeight + 1;

  game_level_object_generator largeAsteroidGenerator(largeLeftColumn, largeRightColumn, largeColumnWidth, largeTopRow, largeBottomRow, largeRowHeight, 0.90f, 1.0f, 7.0f);
  largeAsteroidGenerator.InsertInto(inserter);
}
