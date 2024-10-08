#include "pch.h"
#include "game_world.h"
#include "level_0.h"
#include "level_1.h"
#include "level_2.h"
#include "visitor.h"

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
  auto levelContainer = std::make_unique<level_container>();

  auto levelData = LevelData(levelIndex);
  levelData->Enumerate([this,levelIndex,&levelContainer](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto cellType = m_cellDataTranslator(levelIndex, cellData);
    auto cellId = cell_id { columnIndex, rowIndex };

    switch( cellType )
    {
      case level_cell_type::wall:
      case level_cell_type::floor:
        levelContainer->AddCell(cellId, cellType);
        break;
    }
  });

  levelData->Enumerate([this,&levelContainer,levelIndex](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto itemType = m_objectDataTranslator(levelIndex, cellData);
    auto cellId = cell_id { columnIndex, rowIndex };

    switch( itemType )
    {
      case level_item_type::entry_portal:
        levelContainer->AddObject(level_container::object_type::portal_entry, cellId);
        levelContainer->AddObject(level_container::object_type::player, cellId);
        break;

      case level_item_type::exit_portal:
        levelContainer->AddObject(level_container::object_type::portal_exit, cellId);
        break;

      case level_item_type::enemy_type_one:
        levelContainer->AddObject(level_container::object_type::enemy_stalker, cellId);
        break;

      case level_item_type::enemy_type_two:
        levelContainer->AddObject(level_container::object_type::enemy_random, cellId);
        break;

      case level_item_type::enemy_type_three:
        levelContainer->AddObject(level_container::object_type::enemy_turret, cellId);
        break;

    }
  });

  levelContainer->AddBoundaryWalls();

  return levelContainer;
}

auto game_world::UpdateLevel(level_container &levelContainer) const noexcept -> void
{
#ifdef _ENABLE_ENEMY_SPAWN
  auto unoccupiedFloorCellCount = levelContainer.UnoccupiedFloorCellCount();

  std::uniform_int_distribution<size_t> cellDist { 0, unoccupiedFloorCellCount };
  auto cellIndex = cellDist(pseudo_random_generator::get());

  auto cellId = levelContainer.UnoccupiedFloorCell(cellIndex);

  constexpr size_t objectTypeMax = 4;
  std::uniform_int_distribution<size_t> enemyTypeDist { 0, objectTypeMax };
  auto objectTypeIndex = enemyTypeDist(pseudo_random_generator::get());

  auto objectType = level_container::object_type::enemy_stalker;

  switch( objectTypeIndex )
  {
    case 0:
      objectType = level_container::object_type::enemy_stalker;
      break;
    case 1:
      objectType = level_container::object_type::enemy_random;
      break;
    case 2:
      objectType = level_container::object_type::enemy_turret;
      break;
    case 3:
    case 4:
      objectType = level_container::object_type::power_up;
      break;
  }

  auto& object = levelContainer.AddObject(objectType, cellId);

  object.Visit(visitor {
    [](enemy_type_1& innerObject) { innerObject.SetHitpoints(3); },
    [](enemy_type_2& innerObject) { innerObject.SetHitpoints(3); },
    [](enemy_type_3& innerObject) { innerObject.SetHitpoints(3); },
    [](auto& innerObject) {}
  });
#endif
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
