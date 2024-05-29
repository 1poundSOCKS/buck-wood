#pragma once

#include "level_object_data_translator.h"

class game_world_object_data_translator
{

public:

  [[nodiscard]] auto operator()(int levelIndex, char cellData) const  -> level_item_type
  {
    return m_translator(cellData);
  }

private:

  level_object_data_translator m_translator;

};
