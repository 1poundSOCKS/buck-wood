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

auto game_world::EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>
{
  auto entryIterator = m_links.find({index,exitCell.x, exitCell.y});
  if( entryIterator == std::end(m_links) )
  {
    return std::nullopt;
  }
  else
  {
    const auto& [key, value] = *entryIterator;
    const auto& [levelIndex, column, row] = value;
    return std::make_optional<std::tuple<int, POINT_2I>>(levelIndex, POINT_2I { column, row });
  }
}

auto game_world::LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell) const -> std::unique_ptr<level_container>
{
  auto levelData = LevelData(levelIndex);

  auto levelCells = CreateCellsCollection(levelIndex, levelData.get(), cell_size { 400, 400 });

  auto levelContainer = std::make_unique<level_container>();

  auto cellRect = levelCells->CellRect({0,0});
  auto scale = SCALE_2F { cellRect.right - cellRect.left, cellRect.bottom - cellRect.top };
  
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
      {
        levelContainer->Create(level_container::object_type::portal_entry, cellPosition);
        auto& player = levelContainer->Create(level_container::object_type::player, cellPosition);
        std::visit([levelCells](auto&& object) { SetCells(object, levelCells); }, player.Get());
        break;
      }

      case level_item_type::exit_portal:
      {
        auto& exitPortal = levelContainer->Create(level_container::object_type::portal_exit, cellPosition);
        std::visit([cellId](auto&& object) { SetCellId(object, cellId); }, exitPortal.Get());
        break;
      }

      case level_item_type::enemy_type_one:
      {
        auto& enemy = levelContainer->Create(level_container::object_type::enemy_stalker, cellPosition);
        std::visit([levelCells](auto&& object) { SetCells(object, levelCells); }, enemy.Get());
        break;
      }
      
      case level_item_type::enemy_type_two:
      {
        auto& enemy = levelContainer->Create(level_container::object_type::enemy_random, cellPosition);
        std::visit([levelCells](auto&& object) { SetCells(object, levelCells); }, enemy.Get());
        break;
      }

      case level_item_type::enemy_type_three:
      {
        auto& enemy = levelContainer->Create(level_container::object_type::enemy_turret, cellPosition);
        std::visit([levelCells](auto&& object) { SetCells(object, levelCells); }, enemy.Get());
        break;
      }
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
      case level_cell_type::exit:
        levelCells->Add({columnIndex, rowIndex}, cellType);
        break;
    }
  });

  levelCells->AddWalls();

  return levelCells;
}

auto game_world::CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void
{
  m_cellDataTranslator.AddExit(exitLevelIndex, exitCellDataValue);
  m_objectDataTranslator.AddExit(exitLevelIndex, exitCellDataValue);

  auto levelData = LevelData(exitLevelIndex);
  std::optional<POINT_2I> exitCell;

  levelData->Enumerate([this,exitLevelIndex,exitCellDataValue,&exitCell](size_t column, size_t row, char cellData)
  {
    if( cellData == exitCellDataValue )
    {
      exitCell = POINT_2I { static_cast<int>(column), static_cast<int>(row) };
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
    m_links[std::make_tuple(exitLevelIndex, exitCell->x, exitCell->y)] = std::make_tuple(entryLevelIndex, entryCell->x, entryCell->y);
  }
}
