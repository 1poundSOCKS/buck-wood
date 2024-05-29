#pragma once

#include "level_base.h"
#include "game_world_cell_data_translator.h"
#include "game_world_object_data_translator.h"
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
  static auto SetCellId(portal& object, POINT_2I cellId) -> void;
  static auto SetCellId(auto&& object, POINT_2I cellId) -> void;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  game_world_object_data_translator m_objectDataTranslator;
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
    levelContainer->CreateWall(cell.Position(), scale, 0.0f, cell.Type(), POINT_2I { cell.X(), cell.Y() });
  });

  levelData->Enumerate([this,&levelContainer,levelCells,levelIndex](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto itemType = m_objectDataTranslator(levelIndex, cellData);
    auto cellId = POINT_2I { columnIndex, rowIndex };
    auto cellPosition = levelCells->CellPosition(cellId.x, cellId.y);

    switch( itemType )
    {
      case level_item_type::entry_portal:
        levelContainer->Create(level_container::object_type::portal_entry, cellPosition);
        levelContainer->Create(level_container::object_type::player, cellPosition);
        break;

      case level_item_type::exit_portal:
        {
          auto& exitPortal = levelContainer->Create(level_container::object_type::portal_exit, cellPosition);
          std::visit([cellId](auto&& object) { SetCellId(object, cellId); }, exitPortal.Get());
          break;
        }

      case level_item_type::enemy_type_one:
        levelContainer->Create(level_container::object_type::enemy_stalker, cellPosition);
        break;
      
      case level_item_type::enemy_type_two:
        levelContainer->Create(level_container::object_type::enemy_random, cellPosition);
        break;

      case level_item_type::enemy_type_three:
        levelContainer->Create(level_container::object_type::enemy_turret, cellPosition);
        break;
    }
  });

  return levelContainer;
}

inline auto game_world::SetCellId(portal &object, POINT_2I cellId) -> void
{
  object.SetCellId(cellId);
}

auto game_world::SetCellId(auto &&object, POINT_2I cellId) -> void
{
}
