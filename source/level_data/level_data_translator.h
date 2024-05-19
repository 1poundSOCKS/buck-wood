#pragma once

#include "level_base.h"
#include "level_cell_data_translator.h"
#include "level_object_data_translator.h"

class level_data_translator
{

public:

  auto EnumerateCells(const level_base* levelData, auto&& visitor) const -> void;
  auto EnumerateItems(const level_base* levelData, auto&& visitor) const -> void;

private:

  level_cell_data_translator m_cellDataTranslator;
  level_object_data_translator m_objectDataTranslator;

};

auto level_data_translator::EnumerateCells(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->Enumerate([this,&visitor](size_t column, size_t row, char cellType)
  {
    visitor(column, row, m_cellDataTranslator(cellType));
  });
}

auto level_data_translator::EnumerateItems(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->Enumerate([this,&visitor](size_t column, size_t row, char itemType)
  {
    visitor(column, row, m_objectDataTranslator(itemType));
  });
}
