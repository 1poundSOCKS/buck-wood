#pragma once

#include "level_container.h"

class game_level_data_loader
{
public:

  game_level_data_loader();

  auto LoadLevel() -> std::unique_ptr<level_container>;
  [[nodiscard]] auto NextLevel() -> bool;

private:

  int m_levelIndex { 0 };
  inline static int m_levelCount { 1 };

};
