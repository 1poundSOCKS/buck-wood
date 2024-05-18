#pragma once

#include "level_update_event.h"
#include "level_1.h"
#include "level_2.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  static auto loadLevel(level_base* levelData, auto&&...args) -> std::unique_ptr<level_container>;
  static auto updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;

  static [[nodiscard]] auto moreLevels(int levelIndex) -> bool;
  static [[nodiscard]] auto nextLevel(int levelIndex) -> bool;
  static [[nodiscard]] auto moreUpdates() -> bool;
  static [[nodiscard]] auto levelCanBeCompleted() -> bool;

private:

  game_level_data_loader();

  auto LoadLevel(level_base* levelData, auto&&...args) -> std::unique_ptr<level_container>;
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

inline auto game_level_data_loader::loadLevel(level_base* levelData, auto&&...args) -> std::unique_ptr<level_container>
{
  return m_instance->LoadLevel(levelData, std::forward<decltype(args)>(args)...);
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

auto game_level_data_loader::LoadLevel(level_base* levelData, auto&&...args) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(std::forward<decltype(args)>(args)...);
  levelData->EnumerateCells([&levelContainer](size_t column, size_t row, level_cell_type cellType) -> void
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    
    switch( cellType )
    {
      case level_cell_type::floor:
      case level_cell_type::exit:
        levelContainer->AddFloorCell(columnIndex, rowIndex, cellType);
        break;
    }
  });

  levelData->EnumerateItems([&levelContainer](size_t column, size_t row, level_item_type itemType) -> void
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    
    switch( itemType )
    {
      case level_item_type::portal:
        levelContainer->CreatePortal(POINT_2I { columnIndex, rowIndex });
        levelContainer->CreatePlayer(POINT_2I { columnIndex, rowIndex });
        break;

      case level_item_type::enemy_type_one:
        levelContainer->CreateEnemyType1(POINT_2I { columnIndex, rowIndex }, 10);
        break;
      
      case level_item_type::enemy_type_two:
        levelContainer->CreateEnemyType2(POINT_2I { columnIndex, rowIndex }, 3, 2.0f, 400.0f, 2.0f);
        break;

      case level_item_type::enemy_type_three:
        break;
    }
  });

  levelContainer->AddWalls();
  
  m_status = status::starting;
  m_levelCanBeCompleted = true;

  m_events.clear();
  
  m_currentEvent = std::begin(m_events);

  return levelContainer;
}
