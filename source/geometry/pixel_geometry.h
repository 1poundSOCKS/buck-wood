#pragma once

#include "cell_id.h"
#include "cell_size.h"

class pixel_geometry
{

public:

  pixel_geometry() noexcept = default;
  pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept;

  auto Load(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept -> void;

  [[nodiscard]] operator winrt::com_ptr<ID2D1Geometry>() const noexcept;

private:

  static [[nodiscard]] auto NextPositionClockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position;
  static [[nodiscard]] auto NextPositionAnticlockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position;
  static [[nodiscard]] auto OppositePosition(cell_id::relative_position currentPosition) -> cell_id::relative_position;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};

pixel_geometry::pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept
{
  Load(pixelIds, pixelSize);
}

inline auto pixel_geometry::Load(std::ranges::input_range auto &&pixelIds, cell_size pixelSize) noexcept -> void
{
  std::set<cell_id> pixelIdLookup, borderPixelIdLookup;

  std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));

  // std::ranges::copy_if(pixelIds, std::inserter(borderPixelIdLookup, std::begin(borderPixelIdLookup)), [&pixelIdLookup](cell_id pixelId)
  // {
  //   auto leftId = pixelId.Get(cell_id::relative_position::left);
  //   auto aboveId = pixelId.Get(cell_id::relative_position::above);
  //   auto rightId = pixelId.Get(cell_id::relative_position::right);
  //   auto belowId = pixelId.Get(cell_id::relative_position::below);

  //   return pixelIdLookup.contains(leftId) && pixelIdLookup.contains(aboveId) && pixelIdLookup.contains(rightId) && pixelIdLookup.contains(belowId) ? false : true;
  // });

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

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}

inline auto pixel_geometry::NextPositionClockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position
{
  switch( currentPosition )
  {
    case cell_id::relative_position::left:
      return cell_id::relative_position::above_left;
    case cell_id::relative_position::above_left:
      return cell_id::relative_position::above;
    case cell_id::relative_position::above:
      return cell_id::relative_position::above_right;
    case cell_id::relative_position::above_right:
      return cell_id::relative_position::right;
    case cell_id::relative_position::right:
      return cell_id::relative_position::below_right;
    case cell_id::relative_position::below_right:
      return cell_id::relative_position::below;
    case cell_id::relative_position::below:
      return cell_id::relative_position::below_left;
    case cell_id::relative_position::below_left:
      return cell_id::relative_position::left;
    default:
      return cell_id::relative_position::above;
  }
}

inline auto pixel_geometry::NextPositionAnticlockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position
{
  switch( currentPosition )
  {
    case cell_id::relative_position::left:
      return cell_id::relative_position::below_left;
    case cell_id::relative_position::above_left:
      return cell_id::relative_position::left;
    case cell_id::relative_position::above:
      return cell_id::relative_position::above_left;
    case cell_id::relative_position::above_right:
      return cell_id::relative_position::above;
    case cell_id::relative_position::right:
      return cell_id::relative_position::above_right;
    case cell_id::relative_position::below_right:
      return cell_id::relative_position::right;
    case cell_id::relative_position::below:
      return cell_id::relative_position::below_right;
    case cell_id::relative_position::below_left:
      return cell_id::relative_position::below;
    default:
      return cell_id::relative_position::above;
  }
}

inline auto pixel_geometry::OppositePosition(cell_id::relative_position currentPosition) -> cell_id::relative_position
{
  switch( currentPosition )
  {
    case cell_id::relative_position::left:
      return cell_id::relative_position::right;
    case cell_id::relative_position::above_left:
      return cell_id::relative_position::below_right;
    case cell_id::relative_position::above:
      return cell_id::relative_position::below;
    case cell_id::relative_position::above_right:
      return cell_id::relative_position::below_left;
    case cell_id::relative_position::right:
      return cell_id::relative_position::left;
    case cell_id::relative_position::below_right:
      return cell_id::relative_position::above_left;
    case cell_id::relative_position::below:
      return cell_id::relative_position::above;
    case cell_id::relative_position::below_left:
      return cell_id::relative_position::above_right;
    default:
      return cell_id::relative_position::above;
  }
}
