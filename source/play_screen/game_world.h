#pragma once

#include "level_base.h"

class game_world
{

public:

  [[nodiscard]] auto LevelIndex(int index, POINT_2I exitCell) const -> int;
  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryCell(int index, POINT_2I exitCell) -> POINT_2I;

};
