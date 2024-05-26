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
  auto LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) const -> std::unique_ptr<level_container>;

private:

  [[nodiscard]] auto CreateCellsCollection(int levelIndex, level_base* levelData) const -> std::shared_ptr<level_cell_collection>;
  auto CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  level_object_data_translator m_objectDataTranslator;
  std::map<std::tuple<int, int, int>,std::tuple<int, int, int>> m_links;

};

auto game_world::LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) const -> std::unique_ptr<level_container>
{
  auto levelData = LevelData(levelIndex);

  auto levelCells = CreateCellsCollection(levelIndex, levelData.get());

  auto levelContainer = std::make_unique<level_container>(levelCells, std::forward<decltype(args)>(args)...);

  auto scale = SCALE_2F { static_cast<float>(levelCells->CellWidth()), static_cast<float>(levelCells->CellHeight()) };
  levelCells->EnumerateCells([this,&levelContainer,&scale](const level_cell_item& cell)
  {
    levelContainer->CreateWallObject(std::in_place_type<level_wall>, cell.Position(), scale, 0.0f, cell.Type(), POINT_2I { cell.X(), cell.Y() });
  });

  levelContainer->AddWallCollsionObjects();

  levelData->Enumerate([this,&levelContainer,levelCells](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto itemType = m_objectDataTranslator(cellData);
    auto cellId = POINT_2I { columnIndex, rowIndex };
    auto cellPosition = levelCells->CellPosition(cellId.x, cellId.y);

    switch( itemType )
    {
      case level_item_type::portal:
        levelContainer->CreatePortal(cellPosition);
        levelContainer->CreatePlayer(cellPosition);
        break;

      case level_item_type::enemy_type_one:
        levelContainer->CreateEnemyType1(cellPosition, 10);
        break;
      
      case level_item_type::enemy_type_two:
        levelContainer->CreateEnemyType2(cellPosition, 3, 2.0f, 400.0f, 2.0f);
        break;

      case level_item_type::enemy_type_three:
        levelContainer->CreateEnemyType3(cellPosition, 3, 2.5f);
        break;
    }
  });

  return levelContainer;
}