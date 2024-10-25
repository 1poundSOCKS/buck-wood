#pragma once

#include "cell_id.h"

namespace pixel_geometry_loader
{

  using pixel_id = cell_id;

  struct pixel_data
  {
    int column;
    int row;
    char value;
  };

  struct line_data
  {
    POINT_2I start;
    POINT_2I end;
  };

  auto imageDataToPixelData(std::ranges::input_range auto&& pixelRows, auto pixelDataInserter) -> void;
  auto pixelDataToLineData(std::ranges::input_range auto&& pixelData, auto lineDataInserter) -> void;
  auto lineDataToOrderedPointData(std::ranges::input_range auto&& lineData, cell_size pixelSize, auto pointDataInserter) -> void;
  auto centrePointData(std::ranges::input_range auto&& pointData, auto pointDataInserter) -> void;
  auto alignPointDataBelow(std::ranges::input_range auto&& pointData, float shiftDown, auto pointDataInserter) -> void;
  auto imageDataToOrderedPointData(std::ranges::input_range auto&& pixelRows, cell_size pixelSize, auto pointDataInserter, auto&& pixelCheckFunction);
  auto pixelDataToOrderedPointData(std::ranges::input_range auto&& pixelData, cell_size pixelSize, auto pointDataInserter, auto&& pixelCheckFunction);
  auto getGeometryBounds(std::ranges::input_range auto &&pointData) -> RECT_F;

};

auto pixel_geometry_loader::imageDataToPixelData(std::ranges::input_range auto &&imageData, auto pixelDataInserter) -> void
{
  static constexpr int columnOffset = -1;
  static constexpr int rowOffset = -1;

  int rowIndex = 0;

  for( const auto& rowData : imageData )
  {
    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      pixelDataInserter = pixel_data { columnIndex + columnOffset, rowIndex + rowOffset, rowData[columnIndex] };
    }

    ++rowIndex;
  }
}

auto pixel_geometry_loader::pixelDataToLineData(std::ranges::input_range auto &&pixelData, auto lineDataInserter) -> void
{
  std::set<pixel_id> pixelIdLookup;
  std::ranges::copy(pixelData, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));
  
  for( auto pixelId : pixelIdLookup )
  {
    auto leftId = pixelId.Get(pixel_id::relative_position::left);
    auto aboveId = pixelId.Get(pixel_id::relative_position::above);
    auto rightId = pixelId.Get(pixel_id::relative_position::right);
    auto belowId = pixelId.Get(pixel_id::relative_position::below);

    auto pixelPosition = pixelId.Position(1, 1);

    if( !pixelIdLookup.contains(leftId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y };
      lineDataInserter = line_data { pixelLineStart, pixelLineEnd };
    }

    if( !pixelIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      lineDataInserter = line_data { pixelLineStart, pixelLineEnd };
    }

    if( !pixelIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      lineDataInserter = line_data { pixelLineStart, pixelLineEnd };
    }

    if( !pixelIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      lineDataInserter = line_data { pixelLineStart, pixelLineEnd };
    }
  }
}

auto pixel_geometry_loader::lineDataToOrderedPointData(std::ranges::input_range auto &&lineData, cell_size pixelSize, auto pointDataInserter) -> void
{
  using point_data_container = std::list<POINT_2I>;
  using point_data_container_size = std::ranges::range_difference_t<point_data_container>;
  using point_data_group = std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>;
  using point_data_group_container = std::list<point_data_group>;

  std::map<POINT_2I, POINT_2I> pixelLines;
  point_data_container pointData;
  point_data_group_container pointDataGroups;
  point_data_group_container normalizedPointDataGroups;
  
  std::ranges::transform(lineData, std::inserter(pixelLines, std::begin(pixelLines)), [](auto&& line) -> std::pair<POINT_2I, POINT_2I> { return { line.start, line.end }; });

  auto currentLine = std::begin(pixelLines);
  
  while( currentLine != std::end(pixelLines) )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = pixelSize.CellRect({pixelLineStart.x, pixelLineStart.y});
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

  std::ranges::transform(normalizedPointDataGroups, pointDataInserter, [](auto&& pointDataGroup) { auto&& [prev, current, next] = pointDataGroup; return current; });
}

auto pixel_geometry_loader::centrePointData(std::ranges::input_range auto &&pointData, auto pointDataInserter) -> void
{
  auto geometryBounds = getGeometryBounds(pointData);

  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;

  auto shiftedPoints = std::ranges::views::transform(pointData, [shiftLeft, shiftUp](POINT_2F point) -> POINT_2F
  {
    return { point.x - shiftLeft, point.y - shiftUp };
  });

  std::ranges::copy(shiftedPoints, pointDataInserter);
}

auto pixel_geometry_loader::alignPointDataBelow(std::ranges::input_range auto &&pointData, float shiftDown, auto pointDataInserter) -> void
{
  auto geometryBounds = getGeometryBounds(pointData);

  auto shiftedPoints = std::ranges::views::transform(pointData, [shiftDown](POINT_2F point) -> POINT_2F
  {
    return { point.x, point.y + shiftDown };
  });

  std::ranges::copy(shiftedPoints, pointDataInserter);
}

auto pixel_geometry_loader::imageDataToOrderedPointData(std::ranges::input_range auto &&pixelRows, cell_size pixelSize, auto pointDataInserter, auto&& pixelCheckFunction)
{
  std::vector<pixel_geometry_loader::pixel_data> newPixelData;
  pixel_geometry_loader::imageDataToPixelData(pixelRows, std::back_inserter(newPixelData));

  auto filteredPixelData = std::ranges::views::filter(newPixelData, pixelCheckFunction);
  auto pixelData = std::ranges::views::transform(filteredPixelData, [](auto&& pixelData) -> pixel_id { return { pixelData.column, pixelData.row }; });

  std::vector<pixel_geometry_loader::line_data> lineData;
  pixel_geometry_loader::pixelDataToLineData(pixelData, std::back_inserter(lineData));

  std::vector<POINT_2F> pointData;
  pixel_geometry_loader::lineDataToOrderedPointData(lineData, pixelSize, pointDataInserter);
}

auto pixel_geometry_loader::pixelDataToOrderedPointData(std::ranges::input_range auto &&pixelData, cell_size pixelSize, auto pointDataInserter, auto &&pixelCheckFunction)
{
  auto filteredPixelData = std::ranges::views::filter(pixelData, pixelCheckFunction);
  auto transformedPixelData = std::ranges::views::transform(filteredPixelData, [](auto&& pixelData) -> pixel_id { return { pixelData.column, pixelData.row }; });

  std::vector<pixel_geometry_loader::line_data> lineData;
  pixel_geometry_loader::pixelDataToLineData(transformedPixelData, std::back_inserter(lineData));

  std::vector<POINT_2F> pointData;
  pixel_geometry_loader::lineDataToOrderedPointData(lineData, pixelSize, pointDataInserter);
}

auto pixel_geometry_loader::getGeometryBounds(std::ranges::input_range auto &&pointData) -> RECT_F
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
