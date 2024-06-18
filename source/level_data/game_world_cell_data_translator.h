#pragma once

#include "level_types.h"
#include "level_cell_data_translator.h"

class game_world_cell_data_translator
{

public:

  [[nodiscard]] auto operator()(int levelIndex, char cellData) const  -> level_cell_type
  {
    return m_translator(cellData);
  }

private:

  level_cell_data_translator m_translator;

};
