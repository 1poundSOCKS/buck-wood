#pragma once

#include "level_base.h"
#include "level_cell_data_translator.h"
#include "level_object_data_translator.h"

template <typename cell_data_translator_type, typename object_data_translator_type>
class level_data_translator_template
{

public:

  auto EnumerateCells(const level_base* levelData, auto&& visitor) const -> void;
  auto EnumerateItems(const level_base* levelData, auto&& visitor) const -> void;

private:

  cell_data_translator_type m_cellDataTranslator;
  object_data_translator_type m_objectDataTranslator;

};

template <typename cell_data_translator_type, typename object_data_translator_type>
auto level_data_translator_template<cell_data_translator_type, object_data_translator_type>::EnumerateCells(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->Enumerate([this,&visitor](size_t column, size_t row, char cellType)
  {
    visitor(column, row, m_cellDataTranslator(cellType));
  });
}

template <typename cell_data_translator_type, typename object_data_translator_type>
auto level_data_translator_template<cell_data_translator_type, object_data_translator_type>::EnumerateItems(const level_base* levelData, auto&& visitor) const -> void
{
  levelData->Enumerate([this,&visitor](size_t column, size_t row, char itemType)
  {
    visitor(column, row, m_objectDataTranslator(itemType));
  });
}

using level_data_translator = level_data_translator_template<level_cell_data_translator, level_object_data_translator>;
