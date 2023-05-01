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

  auto LoadLevel(ID2D1RenderTarget* renderTarget) -> std::unique_ptr<level_container>;
  auto NextLevel() -> void;
  [[nodiscard]] auto EndOfLevels() const -> bool;

private:

  [[nodiscard]] auto GetTimeLimit() const -> int;

  int m_levelIndex = 0;
};
