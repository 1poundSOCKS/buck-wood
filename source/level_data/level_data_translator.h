#pragma once

#include "level_base.h"
#include "level_cell_data_translator.h"

class level_data_translator
{

public:

  auto EnumerateCells(const level_base* levelData, auto&& visitor) const -> void;
  auto EnumerateItems(const level_base* levelData, auto&& visitor) const -> void;

private:

  static [[nodiscard]] auto ItemType(char cellData) -> level_item_type;

private:

  level_cell_data_translator m_cellDataTranslator;

};

auto level_data_translator::EnumerateCells(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->EnumerateCells([this,&visitor](size_t column, size_t row, char cellType)
  {
    visitor(column, row, m_cellDataTranslator(cellType));
  });
}

auto level_data_translator::EnumerateItems(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->EnumerateItems([&visitor](size_t column, size_t row, char itemType)
  {
    visitor(column, row, ItemType(itemType));
  });
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
