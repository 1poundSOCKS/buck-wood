#pragma once

#include "level_base.h"
#include "game_world_cell_data_translator.h"
#include "level_object_data_translator.h"
#include "level_container.h"

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>;
  auto LoadLevel(int levelIndex, POINT_2I entryCell, auto&&...args) -> std::unique_ptr<level_container>;

private:

  auto CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  level_object_data_translator m_objectDataTranslator;
  std::map<std::tuple<int, int, int>,std::tuple<int, int, int>> m_links;

};

auto game_world::LoadLevel(int levelIndex, POINT_2I entryCell, auto&&...args) -> std::unique_ptr<level_container>
{
  auto levelContainer = std::make_unique<level_container>(std::forward<decltype(args)>(args)...);

  auto levelData = LevelData(levelIndex);

  levelData->Enumerate([this,levelIndex,&levelContainer](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto cellType = m_cellDataTranslator(levelIndex, cellData);

    switch( cellType )
    {
      case level_cell_type::floor:
        levelContainer->AddFloorCell(columnIndex, rowIndex, cellType);
        break;
      case level_cell_type::exit:
        levelContainer->AddFloorCell(columnIndex, rowIndex, cellType);
        break;
    }
  });

  levelData->Enumerate([this,&levelContainer](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto itemType = m_objectDataTranslator(cellData);

    switch( itemType )
    {
      case level_item_type::portal:
        levelContainer->CreatePortal(POINT_2I { columnIndex, rowIndex });
        break;

      case level_item_type::enemy_type_one:
        levelContainer->CreateEnemyType1(POINT_2I { columnIndex, rowIndex }, 10);
        break;
      
      case level_item_type::enemy_type_two:
        levelContainer->CreateEnemyType2(POINT_2I { columnIndex, rowIndex }, 3, 2.0f, 400.0f, 2.0f);
        break;

      case level_item_type::enemy_type_three:
        levelContainer->CreateEnemyType3(POINT_2I { columnIndex, rowIndex }, 3, 2.5f);
        break;
    }
  });

  levelContainer->AddWalls();
  levelContainer->CreatePlayer(entryCell);

  return levelContainer;
}