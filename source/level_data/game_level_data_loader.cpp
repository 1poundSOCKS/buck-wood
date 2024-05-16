#include "pch.h"
#include "game_level_data_loader.h"

game_level_data_loader::game_level_data_loader() : m_currentLevel { std::make_unique<level_1>() }
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
