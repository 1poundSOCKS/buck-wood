#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"
#include "game_clock.h"

class game_level_data_loader
{
public:

  game_level_data_loader() = default;

  auto LoadLevel(auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, float interval) -> void;
  [[nodiscard]] auto MoreLevels() const -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 3 };
  reload_timer m_levelTimer { 5.0f };
  bool m_updateComplete { false };
  int m_targetsToCreate { 1 };

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  demo_level demoLevel;

  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, demoLevel.BoundaryPoints(), demoLevel.PlayerPosition(), std::forward<decltype(args)>(args)...);

  m_targetsToCreate = 1;
  m_levelTimer = reload_timer { 3.0f };

  return levelContainer;
}

inline auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  if( m_levelTimer.Update(interval) )
  {
    if( m_targetsToCreate > 0 )
    {
      levelContainer->CreateTarget( POINT_2F { 0, 0 }, 4.0f, 10);    
      --m_targetsToCreate;
    }
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
