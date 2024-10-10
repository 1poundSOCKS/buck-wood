#pragma once

#include "level_base.h"

namespace pixel_geometry_loader
{

  auto filter(std::ranges::input_range auto&& pixelIds, auto&& inserter, auto&& pixelCheckFunction);
  auto read(std::ranges::input_range auto&& pixelIds, cell_size pixelSize, auto&& inserter, auto&& pixelCheckFunction) -> void;
  [[nodiscard]] auto read(std::ranges::input_range auto&& pixelIds, cell_size pixelSize, auto&& pixelCheckFunction) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto read(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>;

};

auto pixel_geometry_loader::filter(std::ranges::input_range auto &&pixelIds, auto &&inserter, auto &&pixelCheckFunction)
{
  level_base playerPixelImageReader { pixelIds };

  std::list<cell_id> playerPixelData;

  playerPixelImageReader.Enumerate([&playerPixelData,&inserter,&pixelCheckFunction](int column, int row, char pixelData)
  {
    if( pixelCheckFunction(pixelData) ) inserter = cell_id { column, row };
  });
}

auto pixel_geometry_loader::read(std::ranges::input_range auto &&pixelIds, cell_size pixelSize, auto &&inserter, auto &&pixelCheckFunction) -> void
{
  std::set<cell_id> pixelIdLookup;
  std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));
  
  std::map<POINT_2I, POINT_2I> pixelLines;

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
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }
  }

  std::list<POINT_2F> points;

  auto currentLine = std::begin(pixelLines);
  
  while( points.size() < pixelLines.size() )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = pixelSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    auto pixelRectFloat = ToFloat(pixelRect);
    points.emplace_back(pixelRectFloat.left, pixelRectFloat.top);
    currentLine = pixelLines.find(pixelLineEnd);
  }

  auto geometryBounds = std::accumulate(std::begin(points), std::end(points), RECT_F { 0.0f, 0.0f, 0.0f, 0.0f }, [](RECT_F bounds, POINT_2F point)
  {
    bounds.left = std::min(bounds.left, point.x);
    bounds.top = std::min(bounds.top, point.y);
    bounds.right = std::max(bounds.right, point.x);
    bounds.bottom = std::max(bounds.bottom, point.y);
    return bounds;
  });

  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;

  auto shiftedPoints = std::ranges::views::transform(points, [shiftLeft, shiftUp](POINT_2F point) -> POINT_2F
  {
    return { point.x - shiftLeft, point.y - shiftUp };
  });
}

auto pixel_geometry_loader::read(std::ranges::input_range auto &&pixelIds, cell_size pixelSize, auto &&pixelCheckFunction) -> winrt::com_ptr<ID2D1Geometry>
{
  std::list<cell_id> playerPixelData;
  filter(pixelIds, std::back_inserter(playerPixelData), pixelCheckFunction);
  return read(playerPixelData, pixelSize);
}

[[nodiscard]] auto pixel_geometry_loader::read(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>
{
  std::set<cell_id> pixelIdLookup;
  std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));
  
  std::map<POINT_2I, POINT_2I> pixelLines;

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
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }
  }

  std::list<POINT_2F> points;

  auto currentLine = std::begin(pixelLines);
  
  while( points.size() < pixelLines.size() )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = pixelSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    auto pixelRectFloat = ToFloat(pixelRect);
    points.emplace_back(pixelRectFloat.left, pixelRectFloat.top);
    currentLine = pixelLines.find(pixelLineEnd);
  }

  auto geometryBounds = std::accumulate(std::begin(points), std::end(points), RECT_F { 0.0f, 0.0f, 0.0f, 0.0f }, [](RECT_F bounds, POINT_2F point)
  {
    bounds.left = std::min(bounds.left, point.x);
    bounds.top = std::min(bounds.top, point.y);
    bounds.right = std::max(bounds.right, point.x);
    bounds.bottom = std::max(bounds.bottom, point.y);
    return bounds;
  });

  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;

  auto shiftedPoints = std::ranges::views::transform(points, [shiftLeft, shiftUp](POINT_2F point) -> POINT_2F
  {
    return { point.x - shiftLeft, point.y - shiftUp };
  });

  return direct2d::CreatePathGeometry(d2d_factory::get_raw(), shiftedPoints, D2D1_FIGURE_END_CLOSED);
}
