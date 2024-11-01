#pragma once

#include "level_update_event.h"
#include "fractional_counter.h"
#include "cell_size.h"
#include "level_data.h"
#include "geometry/image_data.h"

class game_level_data_loader
{

public:

  enum class status { starting, started, running, finished };

  static auto create() -> void;
  static auto destroy() -> void;

  static auto loadLevel(int levelIndex, level_container& levelContainer) -> bool;
  static [[nodiscard]] auto testLoadLevel(int levelIndex) -> bool;
  static auto loadEmptyCellData(int levelIndex, auto&& cellDataInserter) noexcept -> void;
  static [[nodiscard]] auto getCellFromPosition(POINT_2F position) noexcept -> cell_id;
  static [[nodiscard]] auto cellsAreVisibleToEachOther(cell_id cellId1, cell_id cell_id2, const std::set<cell_id> &emptyCellLookup) -> bool;

private:

  enum class movement_path_type { none, horizontal, vertical };

  game_level_data_loader();

  [[nodiscard]] auto LoadLevel(int levelIndex, level_container& levelContainer) -> bool;
  [[nodiscard]] auto TestLoadLevel(int levelIndex) -> bool;
  auto LoadEmptyCellData(int levelIndex, auto &&cellDataInserter) noexcept -> void;
  [[nodiscard]] auto LoadObjectData(int levelIndex, const std::set<cell_id>& emptyCellLookup, level_container& levelContainer) -> bool;
  static [[nodiscard]] auto CreateObject(default_object_collection& objectCollection, level_data::object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&;
  static auto GetEnemyMovementPath(movement_path_type pathType, cell_id cellId, const std::set<cell_id>& emptyCellLookup, auto &&pointInserter) noexcept -> void;
  static auto GetEnemyMovementArea(cell_id cellId, const std::set<cell_id>& emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  inline static cell_size m_cellSize { 250, 250 };

  // std::vector<image_data::const_iterator::value_type> m_levelData_0;
  std::map<int, std::vector<image_data::const_iterator::value_type>> m_levelData;
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

inline auto game_level_data_loader::loadEmptyCellData(int levelIndex, auto && cellDataInserter) noexcept -> void
{
  m_instance->LoadEmptyCellData(levelIndex, cellDataInserter);
}

inline auto game_level_data_loader::getCellFromPosition(POINT_2F position) noexcept -> cell_id
{
  return m_instance->m_cellSize.CellId(ToInt(position));
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

inline auto game_level_data_loader::GetEnemyMovementArea(cell_id cellId, const std::set<cell_id> &emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void
{
  pointInserter = ToFloat(m_cellSize.CellPosition(cellId));

  auto cellPosition = ToFloat(cellId.Position());

  for( auto&& emptyCellId : emptyCellLookup )
  {
    auto emptyCellPosition = ToFloat(emptyCellId.Position());

    if( direct2d::GetDistanceBetweenPoints(cellPosition, emptyCellPosition)  < maxDistance )
    {
      pointInserter = ToFloat(m_cellSize.CellPosition(emptyCellId));
    }
  }
}

auto game_level_data_loader::LoadEmptyCellData(int levelIndex, auto&& cellDataInserter) noexcept -> void
{
  std::vector<level_data::cell_data> cellData;
  level_data::LoadCellData(levelIndex, std::back_inserter(cellData));

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellData) -> bool
  {
    return cellData.type == level_data::cell_type::empty;
  });

  std::ranges::transform(emptyCells, cellDataInserter, [](auto&& cellData) -> cell_id
  {
    return { cellData.column, cellData.row };
  });
}
