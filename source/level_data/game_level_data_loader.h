#pragma once

#include "level_update_event.h"
#include "demo_level.h"
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

  auto CreatePlayer(level_container* levelContainer) -> void;
  auto CreateTargets(level_container* levelContainer, int count) -> void;
  auto CreateEnemies(level_container* levelContainer, int count) -> void;
  auto CreatePowerUps(level_container* levelContainer, int count) -> void;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 3 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;

  demo_level m_demoLevel;

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, m_demoLevel.BoundaryPoints(), m_demoLevel.PlayerPosition(), std::forward<decltype(args)>(args)...);

  levelContainer->CreatePortal(POINT_2F {0, 0});

  m_events.clear();

  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreatePlayer(levelContainer); });

  switch( m_levelIndex )
  {
    case 0:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateTargets(levelContainer, 1); });
      break;
    
    case 1:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateTargets(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      break;
    
    case 2:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateTargets(levelContainer, 2); });
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateEnemies(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      break;

    default:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateTargets(levelContainer, 1); });
      break;
  }

  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
