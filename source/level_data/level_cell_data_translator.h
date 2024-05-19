#pragma once

#include "level_types.h"

class level_cell_data_translator
{

public:

  [[nodiscard]] auto operator()(char cellData) const  -> level_cell_type
  {
    switch( cellData )
    {
      case '0':
        return level_cell_type::none;
      case 'E':
        return level_cell_type::exit;
      default:
        return level_cell_type::floor;
    }
  }

};
