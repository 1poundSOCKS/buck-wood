#pragma once

#include "level_update_event.h"
#include "fractional_counter.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto loadLevel(int levelIndex, level_container& levelContainer) -> bool;
  static auto updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  static [[nodiscard]] auto moreLevels(int levelIndex) -> bool;
  static [[nodiscard]] auto nextLevel(int levelIndex) -> bool;
  static [[nodiscard]] auto moreUpdates() -> bool;
  static [[nodiscard]] auto levelCanBeCompleted() -> bool;

private:

  game_level_data_loader();

  [[nodiscard]] auto LoadLevel(int levelIndex, level_container& levelContainer) -> bool;
  auto UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels(int levelIndex) const -> bool;
  [[nodiscard]] auto NextLevel(int levelIndex) -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  [[nodiscard]] auto LevelCanBeCompleted() const -> bool;

  static [[nodiscard]] auto LoadCellData(level_container& levelContainer, int levelIndex) -> bool;
  static [[nodiscard]] auto LoadObjectData(level_container& levelContainer, int levelIndex) -> bool;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  status m_status { status::starting };
  inline static int m_levelCount { 9 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

  fractional_counter m_levelUpdateEvent;

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

inline auto game_level_data_loader::loadLevel(int levelIndex, level_container& levelContainer) -> bool
{
  return m_instance->LoadLevel(levelIndex, levelContainer);
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
