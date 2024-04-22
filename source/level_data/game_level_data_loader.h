#pragma once

#include "level_update_event.h"
#include "demo_level.h"
#include "random_velocity.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  static auto loadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>;
  static auto updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  static [[nodiscard]] auto moreLevels(int levelIndex) -> bool;
  static [[nodiscard]] auto nextLevel(int levelIndex) -> bool;
  static [[nodiscard]] auto moreUpdates() -> bool;
  static [[nodiscard]] auto levelCanBeCompleted() -> bool;
  static [[nodiscard]] auto validCells() -> std::shared_ptr<valid_cell_collection>;

private:

  game_level_data_loader() = default;

  auto LoadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels(int levelIndex) const -> bool;
  [[nodiscard]] auto NextLevel(int levelIndex) -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  [[nodiscard]] auto LevelCanBeCompleted() const -> bool;

  auto CreatePlayer(level_container* levelContainer) -> void;
  auto CreateType1Enemies(level_container* levelContainer, int count, int hitpoints) -> void;
  auto CreateType2Enemies(level_container* levelContainer, int count) -> void;
  auto CreatePowerUps(level_container* levelContainer, int count) -> void;
  auto ValidCells() const -> std::shared_ptr<valid_cell_collection>;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  status m_status { status::starting };
  inline static int m_levelCount { 9 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

  demo_level m_demoLevel;

};

inline auto game_level_data_loader::create() -> void
{
  destroy();
  m_instance = new game_level_data_loader();
}

inline auto game_level_data_loader::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto game_level_data_loader::loadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>
{
  return m_instance->LoadLevel(levelIndex, std::forward<decltype(args)>(args)...);
}

inline auto game_level_data_loader::updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void
{
  m_instance->UpdateLevel(levelIndex, levelContainer, interval);
}

inline [[nodiscard]] auto game_level_data_loader::moreLevels(int levelIndex) -> bool
{
  return m_instance->MoreLevels(levelIndex);
}

inline [[nodiscard]] auto game_level_data_loader::nextLevel(int levelIndex) -> bool
{
  return m_instance->NextLevel(levelIndex);
}

inline [[nodiscard]] auto game_level_data_loader::moreUpdates() -> bool
{
  return m_instance->MoreUpdates();
}

inline [[nodiscard]] auto game_level_data_loader::levelCanBeCompleted() -> bool
{
  return m_instance->LevelCanBeCompleted();
}

inline [[nodiscard]] auto game_level_data_loader::validCells() -> std::shared_ptr<valid_cell_collection>
{
  return m_instance->ValidCells();
}

auto game_level_data_loader::LoadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>
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
