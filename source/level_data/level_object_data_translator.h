#pragma once

#include "level_types.h"

class level_object_data_translator
{

public:

  [[nodiscard]] auto operator()(char cellData) const  -> level_item_type
  {
    switch( cellData )
    {
      case 'P':
        return level_item_type::entry_portal;
      case '1':
        return level_item_type::enemy_type_one;
      case '2':
        return level_item_type::enemy_type_two;
      case '3':
        return level_item_type::enemy_type_three;
      default:
        return level_item_type::none;
    }
  }

};
