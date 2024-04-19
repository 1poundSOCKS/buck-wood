#pragma once

#include "level_update_event.h"
#include "demo_level.h"
#include "random_velocity.h"
#include "game_state.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  game_level_data_loader() = default;

  auto LoadLevel(auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels() const -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  [[nodiscard]] auto LevelCanBeCompleted() const -> bool;

  auto CreatePlayer(level_container* levelContainer) -> void;
  auto CreateType1Enemies(level_container* levelContainer, int count, int hitpoints) -> void;
  auto CreateType2Enemies(level_container* levelContainer, int count) -> void;
  auto CreatePowerUps(level_container* levelContainer, int count) -> void;
  auto ValidCells() const -> const valid_cell_collection&;

private:

  status m_status { status::starting };
  inline static int m_levelCount { 9 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

  demo_level m_demoLevel;

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(m_demoLevel.BoundaryPoints(), std::forward<decltype(args)>(args)...);

  levelContainer->CreatePortal(POINT_2F { 0, 0 });
  levelContainer->CreateBackgroundObject(POINT_2F { -500, -500 });
  levelContainer->CreateBackgroundObject(POINT_2F { 800, -900 });
  levelContainer->CreateBackgroundObject(POINT_2F { -700, 300 });
  levelContainer->CreateBackgroundObject(POINT_2F { 700, 500 });
  m_status = status::starting;
  m_levelCanBeCompleted = false;

  m_events.clear();
  
  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void {
    CreatePlayer(levelContainer);
    m_status = status::started;
  });

  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
