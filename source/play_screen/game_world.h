#pragma once

#include "level_base.h"
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
        return m_translator(cellData);

      case 1:
        return cellData == 'E' ? level_cell_type::floor : m_translator(cellData);

      default:
        return m_translator(cellData);
    }
  }

private:

  int m_levelIndex { -1 };
  level_cell_data_translator m_translator;

};

class game_world_object_data_translator : public level_object_data_translator
{

public:

  auto SetLevelIndex(int value) -> void
  {
    m_levelIndex = value;
  }

  [[nodiscard]] auto operator()(char cellData) const  -> level_item_type
  {
    return m_translator(cellData);
  }

private:

  int m_levelIndex { -1 };
  level_object_data_translator m_translator;

};

class game_world_data_translator : public level_data_translator_template<game_world_cell_data_translator, game_world_object_data_translator>
{

public:

  auto SetLevelIndex(int value) -> void
  {
    m_cellDataTranslator.SetLevelIndex(value);
    m_objectDataTranslator.SetLevelIndex(value);
  }

private:

  int m_levelIndex { -1 };

};

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelIndex(int index, POINT_2I exitCell) const -> int;
  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryCell(int index, POINT_2I exitCell) -> POINT_2I;

private:

  game_world_data_translator m_dataTranslator;
  std::map<int, int> m_levelLinks;
  std::map<int, POINT_2I> m_exits;
  std::map<int, POINT_2I> m_entries;

};
