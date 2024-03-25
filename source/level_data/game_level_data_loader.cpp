#include "pch.h"
#include "game_level_data_loader.h"

auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;
}

[[nodiscard]] auto game_level_data_loader::MoreLevels() const -> bool
{
  return m_levelIndex + 1 < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return m_levelIndex;
}

[[nodiscard]] auto game_level_data_loader::MoreUpdates() const -> bool
{
  return m_currentEvent != std::end(m_events);
}

auto game_level_data_loader::CreatePlayer(level_container* levelContainer) -> void
{
  levelContainer->CreatePlayer(m_demoLevel.PlayerPosition());
}

auto game_level_data_loader::CreateTargets(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateTarget(POINT_2F { 0, 0 }, 4.0f, 10);
  }
}

auto game_level_data_loader::CreateEnemies(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateEnemyTypeOne(POINT_2F { 0, 0 });
  }
}

auto game_level_data_loader::CreatePowerUps(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreatePowerUp(POINT_2F { 0, 0 }, m_randomVelocity.get());
  }
}
