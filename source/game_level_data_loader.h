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

  auto LoadIslands(active_object_container& levelObjectContainer) const -> void;
  auto LoadTargets(active_object_container& levelObjectContainer) const -> void;
  [[nodiscard]] auto LoadPlayer(active_object_container& levelObjectContainer) const -> player_ship::control_data;
  [[nodiscard]] auto LoadTimer(passive_object_container& levelObjectContainer) const -> level_timer::control_data;
  [[nodiscard]] auto LoadState(passive_object_container& levelObjectContainer) const -> level_state::control_data;
  [[nodiscard]] auto GetTimeLimit() const -> int64_t;
  auto NextLevel() -> void;
  [[nodiscard]] auto EndOfLevels() const -> bool;

private:

  auto LoadIslands(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void;
  auto LoadTargets(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void;
  [[nodiscard]] auto LoadPlayer(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> player_ship::control_data;
  [[nodiscard]] auto LoadTimer(const game_level_data& levelData, passive_object_container& levelObjectContainer) const -> level_timer::control_data;

  game_level_data_index::const_iterator m_currentLevelDataIterator;
};