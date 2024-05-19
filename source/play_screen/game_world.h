#pragma once

#include "level_base.h"
#include "level_data_translator.h"

template <int level_index>
class game_world_cell_data_translator_template
{

public:

  [[nodiscard]] auto operator()(char cellData) const  -> level_cell_type
  {
    switch( m_levelIndex )
    {
      case 0:
        return m_translator(cellData);

      case 1:
        return cellData == 'E' ? level_cell_type::floor : m_translator(cellData);

      default:
        return m_translator(cellData);
    }
  }

private:

  int m_levelIndex { level_index };
  level_cell_data_translator m_translator;

};

using game_world_cell_data_translator_0 = game_world_cell_data_translator_template<0>;
using game_world_cell_data_translator_1= game_world_cell_data_translator_template<1>;

class game_world_object_data_translator : public level_object_data_translator
{

public:

  [[nodiscard]] auto operator()(char cellData) const  -> level_item_type
  {
    return m_translator(cellData);
  }

private:

  level_object_data_translator m_translator;

};

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelIndex(int index, POINT_2I exitCell) const -> int;
  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryCell(int index, POINT_2I exitCell) -> POINT_2I;

private:

  using game_world_data_translator_0 = level_data_translator_template<game_world_cell_data_translator_0, game_world_object_data_translator>;
  using game_world_data_translator_1 = level_data_translator_template<game_world_cell_data_translator_1, game_world_object_data_translator>;
  game_world_data_translator_0 m_dataTranslator0;
  game_world_data_translator_1 m_dataTranslator1;
  std::map<int, int> m_levelLinks;

};
