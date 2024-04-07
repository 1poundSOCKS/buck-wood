#include "pch.h"
#include "game_level_data_loader.h"

auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;
}

[[nodiscard]] auto game_level_data_loader::MoreLevels() const -> bool
{
  return game_state::level_index() + 1 < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  game_state::set_level_index(game_state::level_index() + 1);
  return game_state::level_index() < m_levelCount;
}

[[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return game_state::level_index();
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
  levelContainer->CreatePlayer(m_demoLevel.PlayerPosition());
}

auto game_level_data_loader::CreateType1Enemies(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateEnemyType1(POINT_2F { 0, 0 });
  }
}

auto game_level_data_loader::CreateType2Enemies(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateEnemyType2(POINT_2F { 0, 0 }, 0.5f, 10);
  }
}

auto game_level_data_loader::CreatePowerUps(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreatePowerUp(POINT_2F { 0, 0 }, m_randomVelocity.get());
  }
}
