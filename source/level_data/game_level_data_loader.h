#pragma once

#include "level_container_loader.h"
#include "level_update_event.h"
#include "level_1.h"
#include "level_2.h"
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

private:

  inline static game_level_data_loader* m_instance { nullptr };

  status m_status { status::starting };
  inline static int m_levelCount { 9 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

  std::unique_ptr<level_base> m_currentLevel;

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
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(std::forward<decltype(args)>(args)...);

  std::unique_ptr<level_base> level;
  
  switch( levelIndex)
  {
    case 0:
     level = std::make_unique<level_1>();
     break;

    default:
     level = std::make_unique<level_2>();
     break;
  }

  level_container_loader levelContainerLoader(std::move(levelContainer));
  level->Enumerate(levelContainerLoader);
  m_currentLevel = std::move(level);
  levelContainer = std::move(levelContainerLoader);
  levelContainer->AddWalls();
  
  m_status = status::starting;
  m_levelCanBeCompleted = true;

  m_events.clear();
  
  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
