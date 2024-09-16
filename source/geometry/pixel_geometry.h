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

  std::ranges::copy_if(pixelIds, std::inserter(borderPixelIdLookup, std::begin(borderPixelIdLookup)), [&pixelIdLookup](cell_id pixelId)
  {
    auto leftId = pixelId.Get(cell_id::relative_position::left);
    auto aboveId = pixelId.Get(cell_id::relative_position::above);
    auto rightId = pixelId.Get(cell_id::relative_position::right);
    auto belowId = pixelId.Get(cell_id::relative_position::below);

    return pixelIdLookup.contains(leftId) && pixelIdLookup.contains(aboveId) && pixelIdLookup.contains(rightId) && pixelIdLookup.contains(belowId) ? false : true;
  });

  // auto boundaryLeft = std::ranges::views::filter(pixelIds, [&pixelIdLookup](const auto& pixelId)
  // {
  //   auto leftPixelId = pixelId.Get(cell_id::relative_position::left);
  //   return pixelIdLookup.contains(leftPixelId) ? false : true;
  // });

  // std::set<cell_id> boundaryLeftLookup;
  // std::ranges::copy(boundaryLeft, std::inserter(boundaryLeftLookup, std::begin(boundaryLeftLookup)));

  auto firstPixelId = pixelIdLookup.find(cell_id { 0, 0 });
  auto currentPixelId = *firstPixelId;
  auto currentPixelRect = pixelSize.CellRect(currentPixelId);

  std::list<POINT_2F> points;

  auto startPixelId = currentPixelId;
  auto startPixelDirection = cell_id::relative_position::left;

  auto stopPixelDirection = NextPositionAnticlockwise(startPixelDirection);
  auto nextPixelDirection = startPixelDirection;
  auto nextPixelId = startPixelId.Get(nextPixelDirection);

  while( nextPixelDirection != stopPixelDirection && !borderPixelIdLookup.contains(nextPixelId) )
  {
    switch( nextPixelDirection )
    {
      case cell_id::relative_position::left:
        points.emplace_back(static_cast<float>(currentPixelRect.left), static_cast<float>(currentPixelRect.top));
        break;
      case cell_id::relative_position::above:
        points.emplace_back(static_cast<float>(currentPixelRect.right), static_cast<float>(currentPixelRect.top));
        break;
      case cell_id::relative_position::right:
        points.emplace_back(static_cast<float>(currentPixelRect.right), static_cast<float>(currentPixelRect.bottom));
        break;
      case cell_id::relative_position::below:
        points.emplace_back(static_cast<float>(currentPixelRect.left), static_cast<float>(currentPixelRect.bottom));
        break;
    }

    nextPixelDirection = NextPositionClockwise(nextPixelDirection);
    nextPixelId = currentPixelId.Get(nextPixelDirection);
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
