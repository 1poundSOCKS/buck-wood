#include "pch.h"
#include "game_world.h"
#include "visitor.h"

game_world::game_world()
{
}

auto game_world::LoadLevel(int levelIndex) const -> std::unique_ptr<level_container>
{
  auto levelContainer = std::make_unique<level_container>();

  std::vector<level_data::cell_data> cellData;
  level_data::LoadCellData(levelIndex, std::back_inserter(cellData));

  for( auto&& cell : cellData )
  {
    auto cellId = cell_id { cell.column, cell.row };

    switch( cell.type )
    {
      case level_data::cell_type::empty:
        levelContainer->AddCell(cellId, level_cell_type::floor);
        break;
      case level_data::cell_type::boundary:
        levelContainer->AddCell(cellId, level_cell_type::wall);
        break;
    }
  }

  std::vector<level_data::object_data> objectData;
  level_data::LoadObjectData(levelIndex, std::back_inserter(objectData));

  for( auto&& object : objectData )
  {
    auto cellId = cell_id { object.column, object.row };

    switch( object.type )
    {
      case level_data::object_type::player:
        levelContainer->AddObject(level_container::object_type::player, cellId);
        break;

      case level_data::object_type::enemy_stalker:
        levelContainer->AddObject(level_container::object_type::enemy_stalker, cellId);
        break;

      case level_data::object_type::enemy_random:
        levelContainer->AddObject(level_container::object_type::enemy_random, cellId);
        break;

      case level_data::object_type::enemy_turret:
        levelContainer->AddObject(level_container::object_type::enemy_turret, cellId);
        break;

    }
  }

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
