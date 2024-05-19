#pragma once

#include "level_base.h"

class level_data_translator
{

public:

  auto EnumerateCells(const level_base* levelData, auto&& visitor) const -> void;
  auto EnumerateItems(const level_base* levelData, auto&& visitor) const -> void;

private:

  static [[nodiscard]] auto CellType(char cellData) -> level_cell_type;
  static [[nodiscard]] auto ItemType(char cellData) -> level_item_type;

};

auto level_data_translator::EnumerateCells(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->EnumerateCells(visitor);
}

auto level_data_translator::EnumerateItems(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->EnumerateItems(visitor);
}

inline [[nodiscard]] auto level_data_translator::CellType(char cellData) -> level_cell_type
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

inline [[nodiscard]] auto level_data_translator::ItemType(char cellData) -> level_item_type
{
  switch( cellData )
  {
    case 'P':
      return level_item_type::portal;
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
