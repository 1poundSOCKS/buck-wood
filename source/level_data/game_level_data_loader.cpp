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
        m_events.emplace_back(1.0f, [this, levelIndex](level_container* levelContainer) -> void { CreateEnemies(levelContainer); m_levelCanBeCompleted = true; });
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

auto game_level_data_loader::CreatePlayer(level_container* levelContainer) -> void
{
  auto playerStartCell = m_currentLevel->PlayerStartCell();
  auto playerStartPosition = levelContainer->Cells().CellPosition(playerStartCell.x, playerStartCell.y);
  levelContainer->CreatePlayer(playerStartPosition);
}

auto game_level_data_loader::CreatePowerUps(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreatePowerUp({ 0, 0 }, m_randomVelocity.get());
  }
}

auto game_level_data_loader::CreateEnemies(level_container* levelContainer) -> void
{
  for( const auto& enemy : m_currentLevel->Enemies1() )
  {
    const auto& [x, y] = enemy;
    levelContainer->CreateEnemyType1(POINT_2I { x, y }, 10);
  }
  
  for( const auto& enemy : m_currentLevel->Enemies2() )
  {
    const auto& [x, y] = enemy;
    levelContainer->CreateEnemyType2(POINT_2I { x, y }, 3, 2.0f, 400.0f, 2.0f);
  }
}
