#include "pch.h"
#include "game_level_data_loader.h"

auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;

  if( m_status == status::started && m_currentEvent == std::end(m_events) )
  {
    m_events.clear();

    switch( game_state::level_index() )
    {
      case 0:
      default:
        m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreateType1Enemies(levelContainer, 1, 10 * (game_state::level_index() + 1)); m_levelCanBeCompleted = true; });
        m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreateType2Enemies(levelContainer, game_state::level_index() + 1); m_levelCanBeCompleted = true; });
        break;
    }

    m_currentEvent = std::begin(m_events);
    m_status = status::running;
  }
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

auto game_level_data_loader::CreateType1Enemies(level_container* levelContainer, int count, int hitpoints) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateEnemyType1(POINT_2F { 0, 0 }, hitpoints);
  }
}

auto game_level_data_loader::CreateType2Enemies(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreateEnemyType2(POINT_2F { 0, 0 }, 3, 100.0f, 2.0f);
  }
}

auto game_level_data_loader::CreatePowerUps(level_container* levelContainer, int count) -> void
{
  for( int i = 0; i < count; ++i )
  {
    levelContainer->CreatePowerUp(POINT_2F { 0, 0 }, m_randomVelocity.get());
  }
}

auto game_level_data_loader::ValidCellGeometries() const -> const demo_level::valid_cell_collection&
{
  return m_demoLevel.ValidCellGeometries();
}
