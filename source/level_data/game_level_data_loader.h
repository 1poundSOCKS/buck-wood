#pragma once

#include "level_update_event.h"
#include "level_1.h"
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

private:

  game_level_data_loader();

  auto LoadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels(int levelIndex) const -> bool;
  [[nodiscard]] auto NextLevel(int levelIndex) -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  [[nodiscard]] auto LevelCanBeCompleted() const -> bool;

  auto CreatePlayer(level_container* levelContainer) -> void;
  auto CreateEnemies(level_container* levelContainer) -> void;
  auto CreatePowerUps(level_container* levelContainer, int count) -> void;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  status m_status { status::starting };
  inline static int m_levelCount { 9 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

  std::unique_ptr<level_base> m_currentLevel;
  // demo_level m_demoLevel;

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

auto game_level_data_loader::LoadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>
{
  // auto cellView = std::ranges::views::transform(m_demoLevel.Cells(), [](const auto& cell) -> POINT_2I
  auto cellView = std::ranges::views::transform(m_currentLevel->Cells(), [](const auto& cell) -> POINT_2I
  {
    const auto& [x, y] = cell;
    return { x, y };
  });

  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(cellView, std::forward<decltype(args)>(args)...);

  // for( const auto& portal : m_demoLevel.Portals() )
  for( const auto& portal : m_currentLevel->Portals() )
  {
    const auto& [x, y] = portal;
    levelContainer->CreatePortal(POINT_2I { x, y });
  }
  
  m_status = status::starting;
  m_levelCanBeCompleted = false;

  m_events.clear();
  
  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void
  {
    CreatePlayer(levelContainer);
    m_status = status::started;
  });

  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
