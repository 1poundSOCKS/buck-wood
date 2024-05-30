#pragma once

#include "level_types.h"
#include "level_cell_data_translator.h"

class game_world_cell_data_translator
{

public:

  auto AddExit(int levelIndex, char exitCellValue) -> void
  {
    m_exits.emplace(levelIndex, exitCellValue);
  }

  [[nodiscard]] auto operator()(int levelIndex, char cellData) const  -> level_cell_type
  {
    return m_translator(cellData);
  }

private:

  level_cell_data_translator m_translator;
  std::set<std::tuple<int,char>> m_exits;

};
