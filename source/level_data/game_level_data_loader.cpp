#include "pch.h"
#include "game_level_data_loader.h"
#include "level_data.h"

game_level_data_loader::game_level_data_loader() : m_levelUpdateEvent { 2.0f }
{
}

auto game_level_data_loader::UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;

  if( m_status == status::started && m_currentEvent == std::end(m_events) )
  {
    m_events.clear();

    switch( levelIndex )
    {
      case 0:
      default:
        break;
    }

    m_currentEvent = std::begin(m_events);
    m_status = status::running;
  }

  if( m_levelUpdateEvent.Update(interval) >= 1.0f )
  {
    /* UPDATE THE LEVEL CONTAINER */
    m_levelUpdateEvent.Normalize();
  }
}

[[nodiscard]] auto game_level_data_loader::MoreLevels(int levelIndex) const -> bool
{
  return levelIndex + 1 < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::NextLevel(int levelIndex) -> bool
{
  return levelIndex < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::MoreUpdates() const -> bool
{
  return m_currentEvent != std::end(m_events);
}

[[nodiscard]] auto game_level_data_loader::LevelCanBeCompleted() const -> bool
{
  return m_levelCanBeCompleted;
}

auto game_level_data_loader::LoadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  std::vector<POINT_2F> boundaryData;

  if( LoadCellData(levelContainer, levelIndex) && LoadObjectData(levelContainer, levelIndex) && level_data::LoadBoundaryData(levelIndex, std::back_inserter(boundaryData)) )
  {
    levelContainer.AddBoundaryWalls(levelIndex, boundaryData);
    m_status = status::starting;
    m_levelCanBeCompleted = true;
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

auto game_level_data_loader::LoadCellData(level_container &levelContainer, int levelIndex) -> bool
{
  std::vector<level_data::cell_data> cellData;
  
  if( level_data::LoadCellData(levelIndex, std::back_inserter(cellData)) )
  {
    for( auto&& cell : cellData )
    {
      auto cellId = cell_id { cell.column, cell.row };

      switch( cell.type )
      {
        case level_data::cell_type::empty:
          levelContainer.AddCell(cellId, level_cell_type::floor);
          break;
        case level_data::cell_type::boundary:
          levelContainer.AddCell(cellId, level_cell_type::wall);
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

auto game_level_data_loader::LoadObjectData(level_container &levelContainer, int levelIndex) -> bool
{
  std::vector<level_data::object_data> objectData;

  if( level_data::LoadObjectData(levelIndex, std::back_inserter(objectData)) )
  {
    for( auto&& object : objectData )
    {
      auto cellId = cell_id { object.column, object.row };

      switch( object.type )
      {
        case level_data::object_type::player:
          levelContainer.AddObject(level_container::object_type::player, cellId);
          break;

        case level_data::object_type::power_up:
          levelContainer.AddObject(level_container::object_type::power_up, cellId);
          break;

        case level_data::object_type::enemy_stalker:
          levelContainer.AddObject(level_container::object_type::enemy_stalker, cellId);
          break;

        case level_data::object_type::enemy_random:
          levelContainer.AddObject(level_container::object_type::enemy_random, cellId);
          break;

        case level_data::object_type::enemy_turret:
          levelContainer.AddObject(level_container::object_type::enemy_turret, cellId);
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
