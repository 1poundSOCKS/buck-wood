#pragma once

#include "level_container.h"

class game_level_data_loader
{
public:

  using level_ptr = std::unique_ptr<level_container>;

  game_level_data_loader();

  auto LoadLevel() -> level_ptr;
  [[nodiscard]] auto NextLevel() -> bool;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 1 };

};
