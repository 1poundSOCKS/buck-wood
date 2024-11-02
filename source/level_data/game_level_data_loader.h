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

  static auto create(int cellWidth, int cellHeight) -> void;
  static auto destroy() -> void;

  static auto loadLevel(int levelIndex, level_container& levelContainer) -> bool;
  static [[nodiscard]] auto testLoadLevel(int levelIndex) -> bool;
  static auto loadEmptyCellData(int levelIndex, auto&& cellDataInserter) noexcept -> void;
  static [[nodiscard]] auto getCellFromPosition(POINT_2F position) noexcept -> cell_id;
  static [[nodiscard]] auto cellsAreVisibleToEachOther(cell_id cellId1, cell_id cell_id2, const std::set<std::pair<int, int>> &emptyCellLookup) -> bool;

private:

  enum class movement_path_type { none, horizontal, vertical };

  game_level_data_loader(int cellWidth, int cellHeight);

  [[nodiscard]] auto LoadLevel(int levelIndex, level_container& levelContainer) -> bool;
  [[nodiscard]] auto TestLoadLevel(int levelIndex) -> bool;
  auto LoadEmptyCellData(int levelIndex, auto &&cellDataInserter) noexcept -> void;
  [[nodiscard]] auto LoadObjectData(int levelIndex, const std::set<std::pair<int, int>>& emptyCellLookup, level_container& levelContainer) -> bool;
  static [[nodiscard]] auto CreateObject(default_object_collection& objectCollection, level_data::object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&;
  auto GetEnemyMovementPath(movement_path_type pathType, std::pair<int, int> cellId, const std::set<std::pair<int, int>>& emptyCellLookup, auto &&pointInserter) noexcept -> void;
  auto GetEnemyMovementArea(std::pair<int, int> cellId, const std::set<std::pair<int, int>>& emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  cell_size m_cellSize;

};

inline auto game_level_data_loader::create(int cellWidth, int cellHeight) -> void
{
  destroy();
  m_instance = new game_level_data_loader(cellWidth, cellHeight);
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

auto game_level_data_loader::GetEnemyMovementPath(movement_path_type pathType, std::pair<int, int> cellId, const std::set<std::pair<int, int>>& emptyCellLookup, auto&& pointInserter) noexcept -> void
{
  auto&& [column, row] = cellId;

  switch( pathType )
  {
    case movement_path_type::horizontal:
    {
      auto leftCellId = std::pair<int, int>(column - 1, row);

      while( emptyCellLookup.contains(leftCellId) )
      {
        cellId = leftCellId;
        leftCellId = { leftCellId.first - 1, leftCellId.second };
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(cellId.first, cellId.second)));

      auto rightCellId = std::pair<int, int>(column + 1, row);

      while( emptyCellLookup.contains(rightCellId) )
      {
        cellId = rightCellId;
        rightCellId = { rightCellId.first + 1, rightCellId.second };
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(cellId.first, cellId.second)));
      break;
    }

    case movement_path_type::vertical:
    {
      auto topCellId = std::pair<int, int>(column, row - 1);

      while( emptyCellLookup.contains(topCellId) )
      {
        cellId = topCellId;
        topCellId = { topCellId.first, topCellId.second - 1 };
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(cellId.first, cellId.second)));

      auto bottomCellId = std::pair<int, int>(column, row + 1);

      while( emptyCellLookup.contains(bottomCellId) )
      {
        cellId = bottomCellId;
        bottomCellId = { bottomCellId.first, bottomCellId.second + 1 };
      }

      pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(cellId.first, cellId.second)));
      break;
    }

    default:
      break;
  }
}

inline auto game_level_data_loader::GetEnemyMovementArea(std::pair<int, int> cellId, const std::set<std::pair<int, int>> &emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void
{
  auto&& [column, row] = cellId;

  pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(column, row)));

  auto cellPosition = ToFloat(cell_id(column, row).Position());

  for( auto&& emptyCellId : emptyCellLookup )
  {
    auto&& [emptyColumn, emptyRow] = emptyCellId;

    auto emptyCellPosition = ToFloat(cell_id(emptyColumn, emptyRow).Position());

    if( direct2d::GetDistanceBetweenPoints(cellPosition, emptyCellPosition)  < maxDistance )
    {
      pointInserter = ToFloat(m_cellSize.CellPosition(cell_id(emptyColumn, emptyRow)));
    }
  }
}

auto game_level_data_loader::LoadEmptyCellData(int levelIndex, auto&& cellDataInserter) noexcept -> void
{
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyToCellData(levelIndex, std::back_inserter(cellData));

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, type] = cellDataItem;
    return type == level_data::cell_type::empty;
  });

  std::ranges::transform(emptyCells, cellDataInserter, [](auto&& cellDataItem) -> std::pair<int, int>
  {
    auto&& [column, row, type] = cellDataItem;
    return { column, row };
  });
}
