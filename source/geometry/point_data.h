#pragma once

#include "framework.h"

namespace point_data
{
  auto CellsIdsToOrderedBoundaryPoints(std::ranges::input_range auto&& cellIds, int cellWidth, int cellHeight, auto boundaryInserter) -> void;
  auto CellsIdsToBoundaryLines(std::ranges::input_range auto&& cellIds, auto lineInserter) -> void;
  auto LinesToOrderedPoints(std::ranges::input_range auto&& lineData, int cellWidth, int cellHeight, auto pointInserter, auto remainingLineInserter) -> void;
  auto BoundaryPointsToNormalizedBoundaryPoints(std::ranges::input_range auto&& pointData, auto boundaryInserter) -> void;
  auto GetBounds(std::ranges::input_range auto &&pointData) -> RECT_F;

  auto CellsIdsToOrderedBoundaryPoints(std::ranges::input_range auto&& cellIds, int cellWidth, int cellHeight, auto boundaryInserter) -> void
  {
    static_assert(std::is_same_v<std::ranges::range_value_t<decltype(cellIds)>, POINT_2I>);
    
    std::vector<std::pair<POINT_2I, POINT_2I>> lineData;
    CellsIdsToBoundaryLines(cellIds, std::back_inserter(lineData));

    std::vector<POINT_2I> pointData;
    std::vector<std::pair<POINT_2I, POINT_2I>> remainingLineData;
    LinesToOrderedPoints(lineData, cellWidth, cellHeight, std::back_inserter(pointData), std::back_inserter(remainingLineData));

    while( remainingLineData.size() > 0 )
    {
      std::vector<POINT_2I> nextBoundaryPoints;
      std::vector<std::pair<POINT_2I, POINT_2I>> additionalRemainingLineData;
      LinesToOrderedPoints(remainingLineData, cellWidth, cellHeight, std::back_inserter(nextBoundaryPoints), std::back_inserter(additionalRemainingLineData));
      remainingLineData.clear();
      std::ranges::copy(additionalRemainingLineData, std::back_inserter(remainingLineData));
    }

    BoundaryPointsToNormalizedBoundaryPoints(pointData, boundaryInserter);
  }

  auto CellsIdsToBoundaryLines(std::ranges::input_range auto&& cellIds, auto lineInserter) -> void
  {
    static_assert(std::is_same_v<std::ranges::range_value_t<decltype(cellIds)>, POINT_2I>);
    
    std::set<POINT_2I> cellIdLookup;
    std::ranges::copy(cellIds, std::inserter(cellIdLookup, std::begin(cellIdLookup)));
    
    for( auto&& cellId : cellIds )
    {
      auto&& [column, row] = cellId;

      auto leftId = POINT_2I { column - 1, row };
      auto aboveId = POINT_2I { column, row - 1 };
      auto rightId = POINT_2I { column + 1, row };
      auto belowId = POINT_2I { column, row + 1 };

      if( !cellIdLookup.contains(leftId) )
      {
        auto pixelLineStart = POINT_2I { column, row + 1 };
        auto pixelLineEnd = POINT_2I { column, row };
        lineInserter = std::pair<POINT_2I, POINT_2I>(pixelLineStart, pixelLineEnd);
      }

      if( !cellIdLookup.contains(aboveId) )
      {
        auto pixelLineStart = POINT_2I { column, row };
        auto pixelLineEnd = POINT_2I { column + 1, row };
        lineInserter = std::pair<POINT_2I, POINT_2I>(pixelLineStart, pixelLineEnd);
      }

      if( !cellIdLookup.contains(rightId) )
      {
        auto pixelLineStart = POINT_2I { column + 1, row };
        auto pixelLineEnd = POINT_2I { column + 1, row + 1 };
        lineInserter = std::pair<POINT_2I, POINT_2I>(pixelLineStart, pixelLineEnd);
      }

      if( !cellIdLookup.contains(belowId) )
      {
        auto pixelLineStart = POINT_2I { column + 1, row + 1 };
        auto pixelLineEnd = POINT_2I { column, row + 1 };
        lineInserter = std::pair<POINT_2I, POINT_2I>(pixelLineStart, pixelLineEnd);
      }
    }
  }

  auto LinesToOrderedPoints(std::ranges::input_range auto&& lineData, int cellWidth, int cellHeight, auto pointInserter, auto remainingLineInserter) -> void
  {
    static_assert(std::is_same_v<std::ranges::range_value_t<decltype(lineData)>, std::pair<POINT_2I, POINT_2I>>);

    std::map<POINT_2I, POINT_2I> pixelLines;
    std::ranges::copy(lineData, std::inserter(pixelLines, std::begin(pixelLines)));

    auto currentLine = std::begin(pixelLines);
    while( currentLine != std::end(pixelLines) )
    {
      const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
      auto pixelPosition = POINT_2I { pixelLineStart.x * cellWidth, pixelLineStart.y * cellHeight };
      auto pixelRect =  RECT_I { pixelPosition.x - cellWidth / 2, pixelPosition.y - cellHeight / 2, pixelPosition.x + cellWidth / 2, pixelPosition.y + cellHeight / 2 };
      pointInserter = POINT_2I { pixelRect.left, pixelRect.top };
      pixelLines.erase(currentLine);
      currentLine = pixelLines.find(pixelLineEnd);
    }

    std::ranges::copy(pixelLines, remainingLineInserter);
  }

  auto BoundaryPointsToNormalizedBoundaryPoints(std::ranges::input_range auto&& pointData, auto boundaryInserter) -> void
  {
    static_assert(std::is_same_v<std::ranges::range_value_t<decltype(pointData)>, POINT_2I>);

    std::vector<std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>> pointDataGroups;
    for( auto pointIter = std::begin(pointData); pointIter != std::end(pointData); ++pointIter )
    {
      auto previousPoint = ( pointIter == std::begin(pointData) ? std::nullopt : std::optional<POINT_2I>(*std::prev(pointIter)) );
      auto pointIterNext = std::next(pointIter);
      auto nextPoint = ( pointIterNext == std::end(pointData) ? std::nullopt : std::optional<POINT_2I>(*pointIterNext) );
      pointDataGroups.emplace_back(previousPoint, *pointIter, nextPoint);
    }

    std::vector<std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>> normalizedPointDataGroups;
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
    static_assert(std::is_same_v<std::ranges::range_value_t<decltype(pointData)>, POINT_2F>);

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
