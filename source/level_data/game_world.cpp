#include "pch.h"
#include "game_world.h"
#include "level_0.h"
#include "level_1.h"
#include "level_2.h"
#include "visitor.h"

game_world::game_world()
{
}

auto game_world::LoadLevel(int levelIndex) const -> std::unique_ptr<level_container>
{
  auto levelContainer = std::make_unique<level_container>();

  std::vector<pixel_geometry_loader::pixel_data> pixelData;
  level_data::Load(levelIndex, std::back_inserter(pixelData));

  for( auto&& pixel : pixelData )
  {
    auto columnIndex = pixel.column;
    auto rowIndex = pixel.row;
    auto cellType = m_cellDataTranslator(levelIndex, pixel.value);
    auto cellId = cell_id { columnIndex, rowIndex };

    switch( cellType )
    {
      case level_cell_type::wall:
      case level_cell_type::floor:
        levelContainer->AddCell(cellId, cellType);
        break;
    }
  }

  for( auto&& pixel : pixelData )
  {
    auto columnIndex = pixel.column;
    auto rowIndex = pixel.row;
    auto itemType = m_objectDataTranslator(levelIndex, pixel.value);
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
