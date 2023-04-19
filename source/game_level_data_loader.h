#pragma once

#include "game_level_data.h"
#include "level_island.h"
#include "level_target.h"
#include "player_ship.h"
#include "level_timer.h"

class game_level_data_loader
{
public:

  game_level_data_loader();

  auto LoadIslands(std::back_insert_iterator<std::vector<level_island>> islandInserter) const -> void;
  auto LoadTargets(std::back_insert_iterator<std::vector<target_state>> targetInserter) const -> void;
  [[nodiscard]] auto LoadPlayer() const -> std::unique_ptr<player_ship>;
  [[nodiscard]] auto LoadTimer() const -> std::unique_ptr<level_timer>;
  auto NextLevel() -> void;
  [[nodiscard]] auto EndOfLevels() const -> bool;

private:

  auto LoadIslands(const game_level_data& levelData, std::back_insert_iterator<std::vector<level_island>> islandInserter) const -> void;
  auto LoadTargets(const game_level_data& levelData, std::back_insert_iterator<std::vector<target_state>> targetInserter) const -> void;
  [[nodiscard]] auto LoadPlayer(const game_level_data& levelData) const -> std::unique_ptr<player_ship>;
  [[nodiscard]] auto LoadTimer(const game_level_data& levelData) const -> std::unique_ptr<level_timer>;

  game_level_data_index::const_iterator m_currentLevelDataIterator;
};
