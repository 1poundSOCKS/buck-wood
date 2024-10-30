#include "pch.h"
#include "game_level_data_loader.h"
#include "level_data.h"
#include "visitor.h"
#include "cell_path.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  std::vector<POINT_2F> boundaryData;

  if( LoadObjectData(levelContainer, levelIndex) && level_data::LoadBoundaryData(levelIndex, std::back_inserter(boundaryData)) )
  {
    levelContainer.CreateBoundary(levelIndex, boundaryData);
    return true;
  }
  else
  {
    return false;
  }
}

auto game_level_data_loader::TestLoadLevel(int levelIndex) -> bool
{
  return levelIndex < level_data::levelCount;
}

auto game_level_data_loader::LoadObjectData(level_container &levelContainer, int levelIndex) -> bool
{
  std::vector<level_data::object_data> objectData;

  if( level_data::LoadObjectData(levelIndex, std::back_inserter(objectData)) )
  {
    std::set<cell_id> emptyCellLookup;
    LoadEmptyCellData(levelIndex, std::inserter(emptyCellLookup, std::begin(emptyCellLookup)));

    for( auto&& object : objectData )
    {
      auto cellId = cell_id { object.column, object.row };
      auto position = ToFloat(m_cellSize.CellPosition(cellId));
      auto scale = SCALE_2F { 1.0f, 1.0f };
      auto angle = 0.0f;

      auto&& objects = levelContainer.Objects();
      enemy_ship::controller enemyController;
      std::vector<POINT_2F> movementPathPoints;

      switch( object.type )
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
          objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::guard, 400.0f, enemyController);
          break;
      }
    }

    auto visible = cellsAreVisibleToEachOther({0,0}, {1,1}, emptyCellLookup);
    visible;

    return true;
  }
  else
  {
    return false;
  }
}

auto game_level_data_loader::cellsAreVisibleToEachOther(cell_id cellId1, cell_id cellId2, const std::set<cell_id> &emptyCellLookup) -> bool
{
  cell_path cellPath { cellId1, cellId2 };
  return std::accumulate(std::begin(cellPath), std::end(cellPath), true, [&emptyCellLookup](bool visible, auto&& cellId) { return visible && emptyCellLookup.contains(cellId); } );
}
