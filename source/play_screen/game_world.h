#pragma once

#include "level_base.h"
#include "game_world_data_translator.h"

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelIndex(int index, POINT_2I exitCell) const -> int;
  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryCell(int index, POINT_2I exitCell) -> POINT_2I;

private:

  game_world_data_translator m_dataTranslator;
  std::map<int, int> m_levelLinks;
  std::map<int, POINT_2I> m_exits;
  std::map<int, POINT_2I> m_entries;

};
