#include "pch.h"
#include "game_level_data_loader.h"
#include "level_data.h"
#include "visitor.h"

game_level_data_loader::game_level_data_loader() : m_levelUpdateEvent { 2.0f }
{
}

auto game_level_data_loader::UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;

  if( m_levelUpdateEvent.Update(interval) >= 1.0f )
  {
    /* UPDATE THE LEVEL CONTAINER */
    m_levelUpdateEvent.Normalize();
  }
}

[[nodiscard]] auto game_level_data_loader::MoreUpdates() const -> bool
{
  return m_currentEvent != std::end(m_events);
}

auto game_level_data_loader::LoadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  std::vector<POINT_2F> boundaryData;

  if( LoadObjectData(levelContainer, levelIndex) && level_data::LoadBoundaryData(levelIndex, std::back_inserter(boundaryData)) )
  {
    levelContainer.CreateBoundary(levelIndex, boundaryData);
    m_events.clear();
    m_currentEvent = std::begin(m_events);
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
    std::vector<level_data::cell_data> cellData;
    std::set<cell_id> emptyCellLookup;

    level_data::LoadCellData(levelIndex, std::back_inserter(cellData));

    auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellData) -> bool
    {
      return cellData.type == level_data::cell_type::empty;
    });

    std::ranges::transform(emptyCells, std::inserter(emptyCellLookup, std::begin(emptyCellLookup)), [](auto&& cellData) -> cell_id
    {
      return { cellData.column, cellData.row };
    });

    for( auto&& object : objectData )
    {
      auto cellId = cell_id { object.column, object.row };
      auto position = ToFloat(m_cellSize.CellPosition(cellId));
      auto scale = SCALE_2F { 1.0f, 1.0f };
      auto angle = 0.0f;

      auto&& objects = levelContainer.Objects();
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
          objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::stalker, 400.0f, movementPathPoints);
          break;

        case level_data::object_type::enemy_random:
          GetEnemyMovementPath(movement_path_type::vertical, cellId, emptyCellLookup, std::back_inserter(movementPathPoints));
          objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::random, 200.0f, movementPathPoints);
          break;

        case level_data::object_type::enemy_turret:
          GetEnemyMovementPath(movement_path_type::none, cellId, emptyCellLookup, std::back_inserter(movementPathPoints));
          objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::turret, 0.0f, movementPathPoints);
          break;
      }
    }

    return true;
  }
  else
  {
    return false;
  }
}
