#pragma once

#include "framework.h"
#include "geometry/image_data.h"
#include "level_data.h"
#include "cell_size.h"

class boundary_data
{

public:

  static auto create(int cellWidth, int cellHeight) noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get(int index) -> const std::vector<POINT_2F>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

  static [[nodiscard]] auto begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;
  static [[nodiscard]] auto end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;

private:

  boundary_data(int cellWidth, int cellHeight);
  auto Add(int index, int cellWidth, int cellHeight) noexcept -> void;
  static auto CellsToBoundary(std::ranges::input_range auto && cells, int cellWidth, int cellHeight, auto boundaryInserter) -> void;

  inline static boundary_data* m_instance { nullptr };
  std::map<int, std::vector<std::tuple<int, int, level_data::cell_type>>> m_levelData;
  std::map<int, std::vector<POINT_2F>> m_boundaryData;

};

inline auto boundary_data::create(int cellWidth, int cellHeight) noexcept -> void
{
  destroy();
  m_instance = new boundary_data(cellWidth, cellHeight);
}

inline auto boundary_data::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto boundary_data::get(int index) -> const std::vector<POINT_2F> &
{
  return m_instance->m_boundaryData[index];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_levelData.contains(index);
}

inline auto boundary_data::begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::begin(m_instance->m_boundaryData);
}

inline auto boundary_data::end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::end(m_instance->m_boundaryData);
}

inline boundary_data::boundary_data(int cellWidth, int cellHeight)
{
  Add(0, cellWidth, cellHeight);
  Add(1, cellWidth, cellHeight);
  Add(2, cellWidth, cellHeight);
}

auto boundary_data::CellsToBoundary(std::ranges::input_range auto&& cells, int cellWidth, int cellHeight, auto boundaryInserter) -> void
{
  auto emptyCellIds = std::ranges::views::transform(cells, [](auto&& cellDataItem) -> std::pair<int,int>
  {
    auto&& [column, row, value] = cellDataItem;
    return { column, row };
  });

  std::set<std::pair<int,int>> cellIdLookup;
  std::ranges::copy(emptyCellIds, std::inserter(cellIdLookup, std::begin(cellIdLookup)));
  
  using line_data = std::pair<POINT_2I, POINT_2I>;
  std::vector<line_data> lineData;

  for( auto&& cellId : emptyCellIds )
  {
    auto&& [column, row] = cellId;

    auto leftId = std::pair<int,int> { column - 1, row };
    auto aboveId = std::pair<int,int> { column, row - 1 };
    auto rightId = std::pair<int,int> { column + 1, row };
    auto belowId = std::pair<int,int> { column, row + 1 };

    if( !cellIdLookup.contains(leftId) )
    {
      auto pixelLineStart = POINT_2I { column, row + 1 };
      auto pixelLineEnd = POINT_2I { column, row };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { column, row };
      auto pixelLineEnd = POINT_2I { column + 1, row };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { column + 1, row };
      auto pixelLineEnd = POINT_2I { column + 1, row + 1 };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { column + 1, row + 1 };
      auto pixelLineEnd = POINT_2I { column, row + 1 };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }
  }

  std::map<POINT_2I, POINT_2I> pixelLines;
  std::vector<POINT_2I> pointData;
  std::vector<std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>> pointDataGroups;
  std::vector<std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>> normalizedPointDataGroups;
  
  std::ranges::copy(lineData, std::inserter(pixelLines, std::begin(pixelLines)));

  auto currentLine = std::begin(pixelLines);
  auto cellSize = cell_size { cellWidth, cellHeight };

  while( currentLine != std::end(pixelLines) )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = cellSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    pointData.emplace_back(pixelRect.left, pixelRect.top);
    pixelLines.erase(currentLine);
    currentLine = pixelLines.find(pixelLineEnd);
  }

  for( auto pointIter = std::begin(pointData); pointIter != std::end(pointData); ++pointIter )
  {
    auto previousPoint = ( pointIter == std::begin(pointData) ? std::nullopt : std::optional<POINT_2I>(*std::prev(pointIter)) );
    auto pointIterNext = std::next(pointIter);
    auto nextPoint = ( pointIterNext == std::end(pointData) ? std::nullopt : std::optional<POINT_2I>(*pointIterNext) );
    pointDataGroups.emplace_back(previousPoint, *pointIter, nextPoint);
  }

  std::ranges::copy_if(pointDataGroups, std::back_inserter(normalizedPointDataGroups), [](auto&& pointDataGroup)
  {
    auto&& [prev, current, next] = pointDataGroup;
    auto groupIsFull = prev != std::nullopt && next != std::nullopt;
    auto allXAreEqual = groupIsFull && prev->x == current.x && next->x == current.x;
    auto allYAreEqual = groupIsFull && prev->y == current.y && next->y == current.y;
    return allXAreEqual || allYAreEqual ? false : true;
  });

  std::ranges::transform(normalizedPointDataGroups, boundaryInserter, [](auto&& pointDataGroup) -> POINT_2F
  {
    auto&& [prev, current, next] = pointDataGroup;
    return ToFloat(current);
  });
}
