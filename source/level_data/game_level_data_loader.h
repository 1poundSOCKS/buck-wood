#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"
#include "game_clock.h"

class game_level_data_loader
{
public:

  game_level_data_loader() = default;

  template <typename...Args> auto LoadLevel(Args...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, int64_t ticks) -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 3 };
  reload_timer m_levelTimer { 5.0f };

};

template <typename...Args> auto game_level_data_loader::LoadLevel(Args...args) -> std::unique_ptr<level_container>
{
  demo_level demoLevel;

  std::unique_ptr<level_container> levelContainer = m_levelIndex % 2 == 0 ?
    std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, demoLevel.BoundaryPoints(), demoLevel.PlayerPosition(), std::forward<Args>(args)...) :
    // std::make_unique<level_container>(level_container::level_type::vertical_scroller, m_levelIndex, std::array<POINT_2F, 0>(), demoLevel.PlayerPosition(), std::forward<Args>(args)...);
    std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, demoLevel.BoundaryPoints(), demoLevel.PlayerPosition(), std::forward<Args>(args)...);

  for( const auto& targetPosition : demoLevel.TargetPositions() )
  {
    levelContainer->CreateTarget(targetPosition, 4.0f, 10);
  }

  return levelContainer;
}

inline auto game_level_data_loader::UpdateLevel(level_container* levelContainer, int64_t ticks) -> bool
{
  return true;
  // return m_levelTimer.Update(game_clock::getInterval(ticks));
}

inline [[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return m_levelIndex;
}
