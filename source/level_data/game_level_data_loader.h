#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"
#include "game_clock.h"
#include "random_velocity.h"

class game_level_data_loader
{

public:

  enum class state_type { starting, active, finished };

  game_level_data_loader() = default;

  auto LoadLevel(auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels() const -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;

private:

  auto UpdateActiveLevel(level_container* levelContainer, float interval) -> void;

private:

  state_type m_state { state_type::starting };
  int m_levelIndex { -1 };
  inline static int m_levelCount { 99 };
  reload_timer m_levelTimer { 3.0f };
  reload_timer m_targetTimer { 3.0f };
  reload_timer m_powerUpTimer { 7.0f };
  bool m_updateComplete { false };
  int m_targetsToCreate { 1 };
  random_velocity m_randomVelocity { 100, 300 };

  demo_level m_demoLevel;

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, m_demoLevel.BoundaryPoints(), m_demoLevel.PlayerPosition(), std::forward<decltype(args)>(args)...);

  levelContainer->CreatePortal(POINT_2F {0, 0});

  m_targetsToCreate = 1;
  m_levelTimer = reload_timer { 3.0f };
  m_targetsToCreate = m_levelIndex + 1;

  m_levelTimer = { 3.0f };
  m_targetTimer = { 3.0f };
  m_powerUpTimer = { 2.0f };

  m_state = state_type::starting;

  return levelContainer;
}

inline auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  switch( m_state )
  {
    case state_type::starting:
      if( m_levelTimer.Update(interval) )
      {
        m_state = state_type::active;
        levelContainer->CreatePlayer(m_demoLevel.PlayerPosition());
      }
      break;

    case state_type::active:
      UpdateActiveLevel(levelContainer, interval);
      m_state = MoreUpdates() ? state_type::active : state_type::finished;
      break;

    default:
      break;
  }
}

inline [[nodiscard]] auto game_level_data_loader::MoreLevels() const -> bool
{
  return m_levelIndex + 1 < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return m_levelIndex;
}

inline [[nodiscard]] auto game_level_data_loader::MoreUpdates() const -> bool
{
  return m_targetsToCreate > 0 ? true : false;
}

inline auto game_level_data_loader::UpdateActiveLevel(level_container* levelContainer, float interval) -> void
{
  if( m_targetTimer.Update(interval) && m_targetsToCreate > 0 )
  {
    levelContainer->CreateTarget( POINT_2F { 0, 0 }, 4.0f, 10);    
    --m_targetsToCreate;
  }

  if( m_powerUpTimer.Update(interval) )
  {
    levelContainer->CreatePowerUp( POINT_2F { 0, 0 }, m_randomVelocity.get() );
  }
}
