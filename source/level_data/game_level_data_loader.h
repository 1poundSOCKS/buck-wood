#pragma once

#include "level_update_event.h"
#include "demo_level.h"
#include "random_velocity.h"
#include "game_state.h"

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
  auto CreateType1Enemies(level_container* levelContainer, int count) -> void;
  auto CreateType2Enemies(level_container* levelContainer, int count) -> void;
  auto CreatePowerUps(level_container* levelContainer, int count) -> void;

private:

  inline static int m_levelCount { 3 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;

  demo_level m_demoLevel;

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  auto levelIndex = game_state::level_index();

  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(level_container::level_type::arena, levelIndex, m_demoLevel.BoundaryPoints(), m_demoLevel.PlayerPosition(), std::forward<decltype(args)>(args)...);

  levelContainer->CreatePortal(POINT_2F {0, 0});

  m_events.clear();

  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreatePlayer(levelContainer); });

  switch( levelIndex )
  {
    case 0:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateType2Enemies(levelContainer, 1); });
      break;
    
    case 1:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateType2Enemies(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      break;
    
    case 2:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateType2Enemies(levelContainer, 2); });
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateType1Enemies(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      m_events.emplace_back(5.0f, [this](level_container* levelContainer) -> void { CreatePowerUps(levelContainer, 1); });
      break;

    default:
      m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateType2Enemies(levelContainer, 5); });
      break;
  }

  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
