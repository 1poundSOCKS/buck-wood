#pragma once

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
