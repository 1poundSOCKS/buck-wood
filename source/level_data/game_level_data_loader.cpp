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
    levelContainer.AddBoundaryWalls(levelIndex, boundaryData);
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
    for( auto&& object : objectData )
    {
      auto cellId = cell_id { object.column, object.row };
      auto position = ToFloat(m_cellSize.CellPosition(cellId));
      auto scale = SCALE_2F { 1.0f, 1.0f };
      auto angle = 0.0f;

      auto createObjectVisitor = visitor
      {
        [cellId](portal& object) { object.SetCellId(cellId); },
        [position](player_ship& object) { object.State()->SetPosition(position); },
        [cellId](enemy_ship& object) { object.SetCellId(cellId); },
        [](auto& object) {}
      };
      
      CreateObject(levelContainer.Objects(), object.type, position, scale, angle).Visit(createObjectVisitor);
    }

    return true;
  }
  else
  {
    return false;
  }
}

auto game_level_data_loader::CreateObject(default_object_collection& objectCollection, level_data::object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&
{
  static auto dummyObject = default_object { std::in_place_type<power_up>, position, scale, angle, VELOCITY_2F { 0.0f, 0.0f } };

  switch( objectType )
  {
    case level_data::object_type::player:
      return objectCollection.Add(std::in_place_type<player_ship>, position, scale, angle, VELOCITY_2F { 0.0f, 0.0f });

    case level_data::object_type::power_up:
      return objectCollection.Add(std::in_place_type<power_up>, position, scale, angle, VELOCITY_2F { 0.0f, 0.0f });

    case level_data::object_type::enemy_stalker:
      return objectCollection.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::stalker);

    case level_data::object_type::enemy_random:
      return objectCollection.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::random);

    case level_data::object_type::enemy_turret:
      return objectCollection.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::turret);

    default:
      return dummyObject;
  }
}
