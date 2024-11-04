#pragma once

#include "level_update_event.h"
#include "fractional_counter.h"
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
  static [[nodiscard]] auto getCellFromPosition(POINT_2F position) noexcept -> POINT_2I;

private:

  enum class movement_path_type { none, horizontal, vertical };

  game_level_data_loader(int cellWidth, int cellHeight);

  [[nodiscard]] auto LoadLevel(int levelIndex, level_container& levelContainer) -> bool;
  [[nodiscard]] auto TestLoadLevel(int levelIndex) -> bool;
  auto LoadEmptyCellData(int levelIndex, auto &&cellDataInserter) noexcept -> void;
  auto LoadInnerWalls(auto&& innerWalls) -> void;
  [[nodiscard]] auto LoadObjectData(int levelIndex, const std::set<std::pair<int, int>>& emptyCellLookup, level_container& levelContainer) -> bool;
  auto GetEnemyMovementPath(movement_path_type pathType, std::pair<int, int> cellId, const std::set<std::pair<int, int>>& emptyCellLookup, auto &&pointInserter) noexcept -> void;
  auto GetEnemyMovementArea(std::pair<int, int> cellId, const std::set<std::pair<int, int>>& emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void;

private:

  inline static game_level_data_loader* m_instance { nullptr };

  int m_cellWidth;
  int m_cellHeight;

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

inline auto game_level_data_loader::getCellFromPosition(POINT_2F position) noexcept -> POINT_2I
{
  auto integerPosition = ToInt(position);
  auto shiftedPosition = POINT_2I { integerPosition.x + m_instance->m_cellWidth / 2, integerPosition.y + m_instance->m_cellHeight / 2 };
  return { shiftedPosition.x / m_instance->m_cellWidth, shiftedPosition.y / m_instance->m_cellHeight };
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

      pointInserter = ToFloat(POINT_2I { cellId.first * m_cellWidth, cellId.second * m_cellHeight });

      auto rightCellId = std::pair<int, int>(column + 1, row);

      while( emptyCellLookup.contains(rightCellId) )
      {
        cellId = rightCellId;
        rightCellId = { rightCellId.first + 1, rightCellId.second };
      }

      pointInserter = ToFloat(POINT_2I { cellId.first * m_cellWidth, cellId.second * m_cellHeight });
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

      pointInserter = ToFloat(POINT_2I { cellId.first * m_cellWidth, cellId.second * m_cellHeight });

      auto bottomCellId = std::pair<int, int>(column, row + 1);

      while( emptyCellLookup.contains(bottomCellId) )
      {
        cellId = bottomCellId;
        bottomCellId = { bottomCellId.first, bottomCellId.second + 1 };
      }

      pointInserter = ToFloat(POINT_2I { cellId.first * m_cellWidth, cellId.second * m_cellHeight });
      break;
    }

    default:
      break;
  }
}

inline auto game_level_data_loader::GetEnemyMovementArea(std::pair<int, int> cellId, const std::set<std::pair<int, int>> &emptyCellLookup, float maxDistance, auto &&pointInserter) noexcept -> void
{
  auto&& [column, row] = cellId;

  pointInserter = ToFloat(POINT_2I { column * m_cellWidth, row * m_cellHeight });

  auto cellPosition = ToFloat(POINT_2I { cellId.first, cellId.second });

  for( auto&& emptyCellId : emptyCellLookup )
  {
    auto&& [emptyColumn, emptyRow] = emptyCellId;

    auto emptyCellPosition = ToFloat(POINT_2I { emptyColumn, emptyRow });

    if( direct2d::GetDistanceBetweenPoints(cellPosition, emptyCellPosition)  < maxDistance )
    {
      pointInserter = ToFloat(POINT_2I { emptyColumn * m_cellWidth, emptyRow * m_cellHeight });
    }
  }
}

auto game_level_data_loader::LoadEmptyCellData(int levelIndex, auto&& cellDataInserter) noexcept -> void
{
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyCellData(levelIndex, std::back_inserter(cellData));

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

inline auto game_level_data_loader::LoadInnerWalls(auto && innerWalls) -> void
{
}
