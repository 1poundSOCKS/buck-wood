#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"
#include "game_clock.h"
#include "random_velocity.h"

class game_level_data_loader
{

public:

  enum class state_type { inactive, active };

  game_level_data_loader() = default;

  auto LoadLevel(auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, float interval) -> void;
  [[nodiscard]] auto MoreLevels() const -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;

private:

  state_type m_state { state_type::inactive };
  int m_levelIndex { -1 };
  inline static int m_levelCount { 99 };
  reload_timer m_levelTimer { 5.0f };
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

  return levelContainer;
}

inline auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  if( m_levelTimer.Update(interval) )
  {
    switch( m_state )
    {
      case state_type::inactive:
        levelContainer->CreatePlayer(m_demoLevel.PlayerPosition());
        m_state = state_type::active;
        break;

      case state_type::active:
        if( m_targetsToCreate > 0 )
        {
          levelContainer->CreateTarget( POINT_2F { 0, 0 }, 4.0f, 10);    
          --m_targetsToCreate;
        }

      default:
        break;
    }
  }

  if( m_state == state_type::active && m_powerUpTimer.Update(interval) )
  {
    levelContainer->CreatePowerUp( POINT_2F { 0, 0 }, m_randomVelocity.get() );
  }
}

inline [[nodiscard]] auto game_level_data_loader::MoreLevels() const -> bool
{
  return m_levelIndex + 1 < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  m_state = state_type::inactive;
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
