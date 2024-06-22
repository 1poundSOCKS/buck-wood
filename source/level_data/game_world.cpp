#include "pch.h"
#include "game_world.h"
#include "level_0.h"
#include "level_1.h"
#include "level_2.h"

game_world::game_world() : m_collisionType { CollisionType() }
{
  CreateLevelLink(0, 'E', 1, 'P');
  CreateLevelLink(1, 'E', 0, 'P');
  CreateLevelLink(1, 'G', 2, 'P');
  CreateLevelLink(2, 'E', 1, 'P');
}

auto game_world::LevelData(int index) const -> std::unique_ptr<level_base>
{
  switch( index )
  {
    case 0:
      return std::make_unique<level_0>();
      
    case 1:
      return std::make_unique<level_1>();

    case 2:
      return std::make_unique<level_2>();

    default:
      return std::make_unique<level_0>();
  }
}

auto game_world::EntryData(int index, cell_id exitCell) -> std::optional<std::tuple<int, cell_id>>
{
  auto entryIterator = m_links.find({index,exitCell});
  if( entryIterator == std::end(m_links) )
  {
    return std::nullopt;
  }
  else
  {
    const auto& [key, value] = *entryIterator;
    const auto& [levelIndex, column, row] = value;
    return std::make_optional<std::tuple<int, cell_id>>(levelIndex, cell_id { column, row });
  }
}

auto game_world::LoadLevel(int levelIndex, std::optional<cell_id> entryCell) const -> std::unique_ptr<level_container>
{
  auto levelData = LevelData(levelIndex);

  auto cellSize = cell_size { 250, 250 };
  auto levelCells = CreateCellsCollection(levelIndex, levelData.get(), cellSize);
  
  auto levelContainer = std::make_unique<level_container>();

  auto cellRect = levelCells->CellRect({0,0});
  auto scale = SCALE_2F { cellRect.right - cellRect.left, cellRect.bottom - cellRect.top };
  
  levelCells->Enumerate([this,&levelContainer,cellSize,scale](cell_id cellId, level_cell_type cellType)
  {
    switch( cellType )
    {
      case level_cell_type::wall:
        levelContainer->AddWall(cellId, cellType);
        break;
    }
  });

  auto levelCellMovement = std::make_shared<level_object_movement>(levelCells);

  levelData->Enumerate([this,&levelContainer,cellSize,levelCells,levelCellMovement,levelIndex](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto itemType = m_objectDataTranslator(levelIndex, cellData);
    auto cellId = cell_id { columnIndex, rowIndex };
    auto cellPosition = ToFloat(cellSize.CellPosition(cellId));

    switch( itemType )
    {
      case level_item_type::entry_portal:
      {
        levelContainer->AddObject(level_container::object_type::portal_entry, cellPosition, {1,1}, 0, {0,0});
        levelContainer->AddObject(level_container::object_type::player, cellPosition, {1,1}, 0, {0,0});
      }
      break;

      case level_item_type::exit_portal:
      {
        auto& exitPortal = levelContainer->AddObject(level_container::object_type::portal_exit, cellPosition, {1,1}, 0, {0,0});
        std::visit([cellId](auto&& object) { SetCellId(object, cellId); }, exitPortal.Get());
      }
      break;

      case level_item_type::enemy_type_one:
        levelContainer->AddObject(level_container::object_type::enemy_stalker, cellPosition, {1,1}, 0, {0,0});
        break;
      
      case level_item_type::enemy_type_two:
        levelContainer->AddObject(level_container::object_type::enemy_random, cellPosition, {1,1}, 0, {0,0});
        break;

      case level_item_type::enemy_type_three:
        levelContainer->AddObject(level_container::object_type::enemy_turret, cellPosition, {1,1}, 0, {0,0});
        break;
    }
  });

  return levelContainer;
}

auto game_world::CollisionType() -> collision_type
{
  switch( game_settings::collisionDetectionType() )
  {
    case game_settings::collision_detection_type::direct2d:
      return collision_type::direct2d;

    case game_settings::collision_detection_type::basic:
    default:
      return collision_type::boundary;
  }
}

auto game_world::CreateCellsCollection(int levelIndex, level_base *levelData, cell_size cellSize) const -> std::shared_ptr<level_cell_collection>
{
  std::shared_ptr<level_cell_collection> levelCells { std::make_shared<level_cell_collection>(cellSize) };

  levelData->Enumerate([this,levelIndex,levelCells](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto cellType = m_cellDataTranslator(levelIndex, cellData);

    switch( cellType )
    {
      case level_cell_type::floor:
        levelCells->Set({columnIndex, rowIndex}, cellType);
        break;
      case level_cell_type::wall:
        levelCells->Set({columnIndex, rowIndex}, cellType);
        break;
    }
  });

  return levelCells;
}

auto game_world::CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void
{
  m_objectDataTranslator.AddExit(exitLevelIndex, exitCellDataValue);

  auto levelData = LevelData(exitLevelIndex);
  std::optional<cell_id> exitCell;

  levelData->Enumerate([this,exitLevelIndex,exitCellDataValue,&exitCell](size_t column, size_t row, char cellData)
  {
    if( cellData == exitCellDataValue )
    {
      exitCell = cell_id { static_cast<int>(column), static_cast<int>(row) };
    }
  });

  auto levelData1 = LevelData(entryLevelIndex);
  std::optional<POINT_2I> entryCell;

  levelData1->Enumerate([this,entryLevelIndex,entryCellDataValue,&entryCell](size_t column, size_t row, char cellData)
  {
    if( cellData == entryCellDataValue )
    {
      entryCell = POINT_2I { static_cast<int>(column), static_cast<int>(row) };
    }
  });

  if( exitCell && entryCell )
  {
    m_links[std::make_tuple(exitLevelIndex, *exitCell)] = std::make_tuple(entryLevelIndex, entryCell->x, entryCell->y);
  }
}
