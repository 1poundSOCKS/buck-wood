#pragma once

#include "level_types.h"
#include "level_data_translator.h"

class game_world_cell_data_translator
{

public:

  auto SetLevelIndex(int value) -> void
  {
    m_levelIndex = value;
  }

  [[nodiscard]] auto operator()(char cellData) const  -> level_cell_type
  {
    switch( m_levelIndex )
    {
      case 0:
      case 1:
        switch( cellData )
        {
          case 'E':
            return level_cell_type::exit;
          default:
            return m_translator(cellData);
        }
        break;

      default:
        return m_translator(cellData);
    }
  }

private:

  int m_levelIndex { -1 };
  level_cell_data_translator m_translator;

};
