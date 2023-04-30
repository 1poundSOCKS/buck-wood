#pragma once

#include "game_level_data.h"
#include "level_island.h"
#include "level_target.h"
#include "player_ship.h"
#include "level_state.h"
#include "level_container.h"

class game_level_data_loader
{
public:

  game_level_data_loader();

  auto LoadLevel(ID2D1RenderTarget* renderTarget) const -> std::unique_ptr<level_container>;
  auto NextLevel() -> void;
  [[nodiscard]] auto EndOfLevels() const -> bool;

private:

  [[nodiscard]] auto GetTimeLimit() const -> int;

  auto LoadIslands(level_container& levelContainer) const -> void;
  auto LoadTargets(level_container& levelContainer) const -> void;

  [[nodiscard]] auto LoadPlayer(level_container& levelContainer) const -> void;
  auto LoadIslands(const game_level_data& levelData, level_container& levelContainer) const -> void;
  auto LoadTargets(const game_level_data& levelData, level_container& levelContainer) const -> void;
  [[nodiscard]] auto LoadPlayer(const game_level_data& levelData, level_container& levelContainer) const -> void;

  game_level_data_index::const_iterator m_currentLevelDataIterator;
};
