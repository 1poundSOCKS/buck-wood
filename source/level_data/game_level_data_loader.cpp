#include "pch.h"
#include "game_level_data_loader.h"
#include "level_data.h"
#include "visitor.h"
#include "cell_path.h"
#include "boundary_data.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  if( !boundary_data::indexIsValid(levelIndex) )
  {
    return false;
  }

  auto&& levelData = boundary_data::get(levelIndex);

  auto cellData = std::ranges::views::transform(levelData, [](auto&& levelDataItem) -> level_data::cell_data
  {
    auto&& [column,row,value] = levelDataItem;
    return { column, row, level_data::ConvertToCellData(value) };
  });

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, value] = cellDataItem;
    return value == level_data::cell_type::empty;
  });

  std::set<cell_id> emptyCellLookup;
  std::ranges::transform(emptyCells, std::inserter(emptyCellLookup, std::begin(emptyCellLookup)), [](auto&& cellDataItem) -> cell_id
  {
    auto&& [column, row, value] = cellDataItem;
    return { column, row };
  });

  if( LoadObjectData(levelIndex, emptyCellLookup, levelContainer) )
  {
    auto emptyPixelIds = std::ranges::views::transform(emptyCells, [](auto&& emptyCell) -> pixel_geometry_loader::pixel_data
    {
      auto&& [column, row, value] = emptyCell;
      return { column, row, ' ' };
    });

    std::vector<POINT_2I> pointData;
    pixel_geometry_loader::pixelDataToOrderedPointData(emptyPixelIds, m_cellSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value != 'X'; });
    auto boundaryData = std::ranges::views::transform(pointData, [](POINT_2I pointData) -> POINT_2F { return ToFloat(pointData); });
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

auto game_level_data_loader::LoadObjectData(int levelIndex, const std::set<cell_id>& emptyCellLookup, level_container& levelContainer) -> bool
{
  if( !boundary_data::indexIsValid(levelIndex) )
  {
    return false;
  }
  
  auto&& levelData = boundary_data::get(levelIndex);

  auto objectData = std::ranges::views::transform(levelData, [](auto&& levelDataItem) -> level_data::object_data
  {
    auto&& [column,row,value] = levelDataItem;
    return { column, row, level_data::ConvertToObjectData(value) };
  });

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
        objects.Add(std::in_place_type<enemy_ship>, position, { 2.0f, 2.0f }, angle, enemy_ship::type::guard, 400.0f, enemyController);
        break;
    }
  }

  auto visible = cellsAreVisibleToEachOther({0,0}, {1,1}, emptyCellLookup);
  visible;

  return true;
}

auto game_level_data_loader::cellsAreVisibleToEachOther(cell_id cellId1, cell_id cellId2, const std::set<cell_id> &emptyCellLookup) -> bool
{
  cell_path cellPath { cellId1, cellId2 };
  return std::accumulate(std::begin(cellPath), std::end(cellPath), true, [&emptyCellLookup](bool visible, auto&& cellId) { return visible && emptyCellLookup.contains(cellId); } );
}
