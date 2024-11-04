#include "pch.h"
#include "game_level_data_loader.h"
#include "level_data.h"
#include "visitor.h"
#include "boundary_data.h"

game_level_data_loader::game_level_data_loader(int cellWidth, int cellHeight) : m_cellWidth { cellWidth }, m_cellHeight { cellHeight }
{
}

auto game_level_data_loader::LoadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  if( !boundary_data::indexIsValid(levelIndex) )
  {
    return false;
  }

  std::vector<std::pair<int, int>> emptyCells;
  LoadEmptyCellData(levelIndex, std::back_inserter(emptyCells));

  std::set<std::pair<int, int>> emptyCellLookup;
  std::ranges::transform(emptyCells, std::inserter(emptyCellLookup, std::begin(emptyCellLookup)), [](auto&& cellDataItem) -> std::pair<int, int>
  {
    auto&& [column, row] = cellDataItem;
    return { column, row };
  });

  levelContainer.Objects().Add(std::in_place_type<boundary_walls>, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, levelIndex);

  std::vector<int> innerWalls;
  LoadInnerWalls(std::back_inserter(innerWalls));

  for( auto innerWall : innerWalls )
  {
    levelContainer.Objects().Add(std::in_place_type<inner_walls>, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, innerWall);
  }

  return LoadObjectData(levelIndex, emptyCellLookup, levelContainer);
}

auto game_level_data_loader::TestLoadLevel(int levelIndex) -> bool
{
  return levelIndex < level_data::levelCount;
}

auto game_level_data_loader::LoadObjectData(int levelIndex, const std::set<std::pair<int, int>>& emptyCellLookup, level_container& levelContainer) -> bool
{
  if( !boundary_data::indexIsValid(levelIndex) )
  {
    return false;
  }
  
  std::vector<std::tuple<int, int, level_data::object_type>> objectData;
  level_data::CopyObjectData(levelIndex, std::back_inserter(objectData));

  for( auto&& object : objectData )
  {
    auto&& [column, row, type] = object;
    auto cellId = std::pair<int, int>(column, row);
    auto position = ToFloat(POINT_2I { cellId.first * m_cellWidth, cellId.second * m_cellHeight });
    auto scale = SCALE_2F { 1.0f, 1.0f };
    auto angle = 0.0f;

    auto&& objects = levelContainer.Objects();
    enemy_ship::controller enemyController;
    std::vector<POINT_2F> movementPathPoints;

    switch( type )
    {
      case level_data::object_type::player:
        objects.Add(std::in_place_type<player_ship>, position, scale, angle, VELOCITY_2F { 0.0f, 0.0f });
        break;

      case level_data::object_type::power_up:
        objects.Add(std::in_place_type<power_up>, position, scale, angle, VELOCITY_2F { 0.0f, 0.0f });
        break;

      case level_data::object_type::enemy_stalker:
        GetEnemyMovementPath(movement_path_type::horizontal, cellId, emptyCellLookup, std::back_inserter(movementPathPoints));
        enemyController = enemy_ship::controller { std::in_place_type<enemy_path>, movementPathPoints };
        objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::stalker, 400.0f, enemyController);
        break;

      case level_data::object_type::enemy_random:
        GetEnemyMovementPath(movement_path_type::vertical, cellId, emptyCellLookup, std::back_inserter(movementPathPoints));
        enemyController = enemy_ship::controller { std::in_place_type<enemy_path>, movementPathPoints };
        objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::random, 200.0f, enemyController);
        break;

      case level_data::object_type::enemy_turret:
        enemyController = enemy_ship::controller { std::in_place_type<enemy_fixed> };
        objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::turret, 0.0f, enemyController);
        break;

      case level_data::object_type::enemy_guard:
        GetEnemyMovementArea(cellId, emptyCellLookup, 5.0f, std::back_inserter(movementPathPoints));
        enemyController = enemy_ship::controller { std::in_place_type<enemy_area>, movementPathPoints };
        objects.Add(std::in_place_type<enemy_ship>, position, { 2.0f, 2.0f }, angle, enemy_ship::type::guard, 400.0f, enemyController);
        break;
    }
  }

  return true;
}
