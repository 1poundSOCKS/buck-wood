#pragma once

#include "game_level_data.h"
#include "level_island.h"
#include "level_target.h"
#include "player_ship.h"
#include "level_timer.h"
#include "level_state.h"
#include "active_object_container.h"
#include "passive_object_container.h"

class game_level_data_loader
{
public:

  game_level_data_loader();

  auto SetPlayerPositionUpdate(player_ship::position_update playerPositionUpdate) -> void;
  auto SetPlayerShot(player_ship::event_shot playerShot) -> void;
  auto SetPlayerDied(player_ship::event_died playerDied) -> void;
  auto SetTargetActivated(level_target::event_activated targetActivated) -> void;
  auto SetTimerUpdate(level_timer::timer_update timerUpdate) -> void;

  auto LoadLevel(active_object_container& levelObjectContainer, passive_object_container& overlayObjectContainer) const -> void;

  auto LoadIslands(active_object_container& levelObjectContainer) const -> void;
  auto LoadTargets(active_object_container& levelObjectContainer) const -> void;

  [[nodiscard]] auto LoadPlayer(active_object_container& levelObjectContainer) const -> void;
  [[nodiscard]] auto LoadTimer(passive_object_container& levelObjectContainer) const -> void;

  auto NextLevel() -> void;
  [[nodiscard]] auto EndOfLevels() const -> bool;

private:

  [[nodiscard]] auto GetTimeLimit() const -> int64_t;
  auto LoadIslands(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void;
  auto LoadTargets(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void;
  [[nodiscard]] auto LoadPlayer(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void;
  [[nodiscard]] auto LoadTimer(const game_level_data& levelData, passive_object_container& levelObjectContainer) const -> void;

  player_ship::position_update m_playerPositionUpdate;
  player_ship::event_shot m_playerShot;
  player_ship::event_died m_playerDied;
  level_target::event_activated m_targetActivated;
  level_timer::timer_update m_timerUpdate;

  game_level_data_index::const_iterator m_currentLevelDataIterator;
};
