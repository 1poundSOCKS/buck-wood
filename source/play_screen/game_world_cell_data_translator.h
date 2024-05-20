#pragma once

#include "level_types.h"
#include "level_cell_data_translator.h"

class game_world_cell_data_translator
{

public:

  auto SetLevelIndex(int value) -> void
  {
    m_levelIndex = value;
  }

  auto AddExit(int levelIndex, char exitCellValue) -> void
  {
    m_exits.emplace(levelIndex, exitCellValue);
  }

  [[nodiscard]] auto operator()(char cellData) const  -> level_cell_type
  {
    auto exitDataIterator = m_exits.find({m_levelIndex, cellData});
    return exitDataIterator == std::end(m_exits) ? m_translator(cellData) : level_cell_type::exit;
  }

private:

  int m_levelIndex { -1 };
  level_cell_data_translator m_translator;
  std::set<std::tuple<int,char>> m_exits;

};
