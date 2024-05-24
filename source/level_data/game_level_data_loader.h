#pragma once

#include "level_update_event.h"
#include "level_1.h"
#include "level_2.h"
#include "game_world.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto entryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>;
  [[nodiscard]] static auto loadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) -> std::unique_ptr<level_container>;
  static auto updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  static [[nodiscard]] auto moreLevels(int levelIndex) -> bool;
  static [[nodiscard]] auto nextLevel(int levelIndex) -> bool;
  static [[nodiscard]] auto moreUpdates() -> bool;
  static [[nodiscard]] auto levelCanBeCompleted() -> bool;

private:

  game_level_data_loader();

  [[nodiscard]] auto EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>;
  auto LoadLevel(int levelIndex, auto&&...args) -> std::unique_ptr<level_container>;
  auto LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto PortalCell(const level_base* levelData) const -> POINT_2I;
  [[nodiscard]] auto MoreLevels(int levelIndex) const -> bool;
  [[nodiscard]] auto NextLevel(int levelIndex) -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  [[nodiscard]] auto LevelCanBeCompleted() const -> bool;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  game_world m_gameWorld;
  status m_status { status::starting };
  inline static int m_levelCount { 9 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;
  bool m_levelCanBeCompleted { false };

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

inline auto game_level_data_loader::entryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>
{
  return m_instance->EntryData(index, exitCell);
}

auto game_level_data_loader::loadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) -> std::unique_ptr<level_container>
{
  return m_instance->LoadLevel(levelIndex, entryCell, std::forward<decltype(args)>(args)...);
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

inline auto game_level_data_loader::EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>
{
  return m_gameWorld.EntryData(index, exitCell);
}

auto game_level_data_loader::LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell, auto&&...args) -> std::unique_ptr<level_container>
{
  auto levelContainer = m_gameWorld.LoadLevel(levelIndex, entryCell, std::forward<decltype(args)>(args)...);

  m_status = status::starting;
  m_levelCanBeCompleted = true;

  m_events.clear();
  
  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
