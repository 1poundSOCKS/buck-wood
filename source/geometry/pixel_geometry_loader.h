#pragma once

#include "level_base.h"

namespace pixel_geometry_loader
{

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
  auto cellDataToLineData(std::ranges::input_range auto&& cellData, auto lineDataInserter) -> void;
  auto lineDataToOrderedPointData(std::ranges::input_range auto&& lineData, cell_size pixelSize, auto pointDataInserter) -> void;
  auto centrePointData(std::ranges::input_range auto&& pointData, auto pointDataInserter) -> void;
  auto imageDataToOrderedPointData(std::ranges::input_range auto&& pixelRows, cell_size pixelSize, auto pointDataInserter, auto&& pixelCheckFunction);

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

auto pixel_geometry_loader::cellDataToLineData(std::ranges::input_range auto &&cellData, auto lineDataInserter) -> void
{
  std::set<cell_id> pixelIdLookup;
  std::ranges::copy(cellData, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));
  
  for( auto pixelId : pixelIdLookup )
  {
    auto leftId = pixelId.Get(cell_id::relative_position::left);
    auto aboveId = pixelId.Get(cell_id::relative_position::above);
    auto rightId = pixelId.Get(cell_id::relative_position::right);
    auto belowId = pixelId.Get(cell_id::relative_position::below);

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
  std::map<POINT_2I, POINT_2I> pixelLines;
  std::ranges::transform(lineData, std::inserter(pixelLines, std::begin(pixelLines)), [](auto&& line) -> std::pair<POINT_2I, POINT_2I> { return { line.start, line.end }; });

  auto lineDataSize = std::ranges::distance(lineData);
  int pointDataInserts = 0;

  auto currentLine = std::begin(pixelLines);
  
  while( pointDataInserts < lineDataSize )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = pixelSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    pointDataInserter = POINT_2I { pixelRect.left, pixelRect.top };
    ++pointDataInserts;
    currentLine = pixelLines.find(pixelLineEnd);
  }
}

auto pixel_geometry_loader::centrePointData(std::ranges::input_range auto &&pointData, auto pointDataInserter) -> void
{
  auto geometryBounds = std::accumulate(std::begin(pointData), std::end(pointData), RECT_F { 0.0f, 0.0f, 0.0f, 0.0f }, [](RECT_F bounds, POINT_2F point)
  {
    bounds.left = std::min(bounds.left, point.x);
    bounds.top = std::min(bounds.top, point.y);
    bounds.right = std::max(bounds.right, point.x);
    bounds.bottom = std::max(bounds.bottom, point.y);
    return bounds;
  });

  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;

  auto shiftedPoints = std::ranges::views::transform(pointData, [shiftLeft, shiftUp](POINT_2F point) -> POINT_2F
  {
    return { point.x - shiftLeft, point.y - shiftUp };
  });

  std::ranges::copy(shiftedPoints, pointDataInserter);
}

auto pixel_geometry_loader::imageDataToOrderedPointData(std::ranges::input_range auto &&pixelRows, cell_size pixelSize, auto pointDataInserter, auto&& pixelCheckFunction)
{
  std::vector<pixel_geometry_loader::pixel_data> newPixelData;
  pixel_geometry_loader::imageDataToPixelData(pixelRows, std::back_inserter(newPixelData));

  auto filteredPixelData = std::ranges::views::filter(newPixelData, pixelCheckFunction);
  auto cellData = std::ranges::views::transform(filteredPixelData, [](auto&& pixelData) -> cell_id { return { pixelData.column, pixelData.row }; });

  std::vector<pixel_geometry_loader::line_data> lineData;
  pixel_geometry_loader::cellDataToLineData(cellData, std::back_inserter(lineData));

  std::vector<POINT_2F> pointData;
  pixel_geometry_loader::lineDataToOrderedPointData(lineData, pixelSize, pointDataInserter);
}
