#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"

class game_level_data_loader
{
public:

  game_level_data_loader() = default;

  template <typename...Args> auto LoadLevel(Args...args) -> std::unique_ptr<level_container>;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 3 };

};

template <typename...Args> auto game_level_data_loader::LoadLevel(Args...args) -> std::unique_ptr<level_container>
{
  demo_level demoLevel;

  level_parameters levelParameters { m_levelIndex, 200.0f, 400.0f, 5, 4.0f / ( m_levelIndex + 1 ) };
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(levelParameters, demoLevel.BoundaryPoints(), demoLevel.PlayerPosition(), std::forward<Args>(args)...);
  levelContainer->AddTargets(demoLevel.TargetPositions());
  return levelContainer;
}

inline [[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return m_levelIndex;
}
