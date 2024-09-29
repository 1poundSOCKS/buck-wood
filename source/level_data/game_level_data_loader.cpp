#include "pch.h"
#include "game_level_data_loader.h"

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
    m_gameWorld.UpdateLevel(*levelContainer);
    m_levelUpdateEvent.Normalize();
  }
}

auto game_level_data_loader::PortalCell(const level_base *levelData) const -> POINT_2I
{
  POINT_2I portalCell { 0, 0 };

  level_object_data_translator levelObjectDataTranslator;
  levelData->Enumerate([&portalCell,&levelObjectDataTranslator](size_t column, size_t row, char cellData) -> void
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);

    auto objectType = levelObjectDataTranslator(cellData);
    
    switch( objectType )
    {
      case level_item_type::entry_portal:
        portalCell = { columnIndex, rowIndex };
        break;
    }
  });

  return portalCell;
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
