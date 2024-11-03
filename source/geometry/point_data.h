#pragma once

#include "framework.h"

namespace point_data
{

  auto CellsToBoundary(std::ranges::input_range auto&& cells, int cellWidth, int cellHeight, auto boundaryInserter) -> void
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

    while( currentLine != std::end(pixelLines) )
    {
      const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
      auto pixelPosition = POINT_2I { pixelLineStart.x * cellWidth, pixelLineStart.y * cellHeight };
      auto pixelRect =  RECT_I { pixelPosition.x - cellWidth / 2, pixelPosition.y - cellHeight / 2, pixelPosition.x + cellWidth / 2, pixelPosition.y + cellHeight / 2 };
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

  auto GetBounds(std::ranges::input_range auto &&pointData) -> RECT_F
  {
    return std::accumulate(std::begin(pointData), std::end(pointData), RECT_F { 0.0f, 0.0f, 0.0f, 0.0f }, [](RECT_F bounds, POINT_2F point)
    {
      bounds.left = std::min(bounds.left, point.x);
      bounds.top = std::min(bounds.top, point.y);
      bounds.right = std::max(bounds.right, point.x);
      bounds.bottom = std::max(bounds.bottom, point.y);
      return bounds;
    });
  }

}
