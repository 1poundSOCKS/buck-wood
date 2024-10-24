#pragma once

#include "level_update_event.h"
#include "fractional_counter.h"
#include "cell_size.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  static auto loadLevel(int levelIndex, level_container& levelContainer) -> bool;
  [[nodiscard]] static auto testLoadLevel(int levelIndex) -> bool;
  static auto updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;
  static [[nodiscard]] auto moreUpdates() -> bool;

private:

  enum class movement_path_type { none, horizontal, vertical };

  game_level_data_loader();

  [[nodiscard]] auto LoadLevel(int levelIndex, level_container& levelContainer) -> bool;
  [[nodiscard]] auto TestLoadLevel(int levelIndex) -> bool;
  auto UpdateLevel(int levelIndex, level_container* levelContainer, float interval) -> void;
  [[nodiscard]] auto MoreUpdates() const -> bool;
  static [[nodiscard]] auto LoadObjectData(level_container& levelContainer, int levelIndex) -> bool;
  static [[nodiscard]] auto CreateObject(default_object_collection& objectCollection, level_data::object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&;
  static auto GetEnemyMovementPath(movement_path_type pathType, cell_id cellId, const std::set<cell_id>& emptyCellLookup, auto &&pointInserter) noexcept -> void;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;

  fractional_counter m_levelUpdateEvent;

  inline static cell_size m_cellSize { 250, 250 };

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

inline auto game_level_data_loader::testLoadLevel(int levelIndex) -> bool
{
  return m_instance->TestLoadLevel(levelIndex);
}

inline auto game_level_data_loader::updateLevel(int levelIndex, level_container* levelContainer, float interval) -> void
{
  m_instance->UpdateLevel(levelIndex, levelContainer, interval);
}

inline [[nodiscard]] auto game_level_data_loader::moreUpdates() -> bool
{
  return m_instance->MoreUpdates();
}

auto game_level_data_loader::GetEnemyMovementPath(movement_path_type pathType, cell_id cellId, const std::set<cell_id>& emptyCellLookup, auto&& pointInserter) noexcept -> void
{
  switch( pathType )
  {
    case movement_path_type::horizontal:
    {
      auto leftCellId = cellId.ShiftColumn(-1);

      while( emptyCellLookup.contains(leftCellId) )
      {
        cellId = leftCellId;
        leftCellId = leftCellId.ShiftColumn(-1);
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cellId));

      auto rightCellId = cellId.ShiftColumn(1);

      while( emptyCellLookup.contains(rightCellId) )
      {
        cellId = rightCellId;
        rightCellId = rightCellId.ShiftColumn(1);
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cellId));
      break;
    }

    case movement_path_type::vertical:
    {
      auto leftCellId = cellId.ShiftRow(-1);

      while( emptyCellLookup.contains(leftCellId) )
      {
        cellId = leftCellId;
        leftCellId = leftCellId.ShiftRow(-1);
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cellId));

      auto rightCellId = cellId.ShiftRow(1);

      while( emptyCellLookup.contains(rightCellId) )
      {
        cellId = rightCellId;
        rightCellId = rightCellId.ShiftRow(1);
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cellId));
      break;
    }

    default:
      break;
  }
}
