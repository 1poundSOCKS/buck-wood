#include "pch.h"
#include "boundary_data.h"
#include "cell_size.h"

auto boundary_data::Add(int index, int cellWidth, int cellHeight) noexcept -> void
{
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyToCellData(index, std::back_inserter(cellData));

  m_levelData.insert({index, cellData});

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, type] = cellDataItem;
    return type == level_data::cell_type::empty;
  });

  using cell_id = std::pair<int,int>;

  auto emptyCellIds = std::ranges::views::transform(emptyCells, [](auto&& cellDataItem) -> cell_id
  {
    auto&& [column, row, value] = cellDataItem;
    return { column, row };
  });

  std::set<cell_id> cellIdLookup;
  std::ranges::copy(emptyCellIds, std::inserter(cellIdLookup, std::begin(cellIdLookup)));
  
  using line_data = std::pair<POINT_2I, POINT_2I>;
  std::vector<line_data> lineData;

  for( auto&& cellId : emptyCellIds )
  {
    auto&& [column, row] = cellId;

    auto leftId = cell_id { column - 1, row };
    auto aboveId = cell_id { column, row - 1 };
    auto rightId = cell_id { column + 1, row };
    auto belowId = cell_id { column, row + 1 };

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

  using point_data_group = std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>;
  using point_data_group_container = std::list<point_data_group>;

  std::map<POINT_2I, POINT_2I> pixelLines;
  std::list<POINT_2I> pointData;
  point_data_group_container pointDataGroups;
  point_data_group_container normalizedPointDataGroups;
  
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

  std::vector<POINT_2F> floatingPointData;
  std::ranges::transform(normalizedPointDataGroups, std::back_inserter(floatingPointData), [](auto&& pointDataGroup) -> POINT_2F
  {
    auto&& [prev, current, next] = pointDataGroup;
    return ToFloat(current);
  });

  m_boundaryData[index] = floatingPointData;
}
