#pragma once

#include "game_world_cell_data_translator.h"

class game_world_data_translator : public level_data_translator_template<game_world_cell_data_translator, level_object_data_translator>
{

public:

  auto SetLevelIndex(int value) -> void
  {
    m_cellDataTranslator.SetLevelIndex(value);
  }

  auto AddExit(int levelIndex, char exitCellValue) -> void
  {
    m_cellDataTranslator.AddExit(levelIndex, exitCellValue);
  }

private:

  int m_levelIndex { -1 };

};
