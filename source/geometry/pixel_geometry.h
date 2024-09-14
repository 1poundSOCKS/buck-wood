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

  auto LoadNext(cell_id firstPixelId, cell_size pixelSize, std::ranges::input_range auto &&pixelPositions, const std::set<cell_id> &pixelIdLookup, auto backInserter) -> void;
  static [[nodiscard]] auto NextPositionClockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};

pixel_geometry::pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept
{
  Load(pixelIds, pixelSize);
}

inline auto pixel_geometry::Load(std::ranges::input_range auto &&pixelIds, cell_size pixelSize) noexcept -> void
{
  std::set<cell_id> pixelIdLookup;
  std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));

  // auto boundaryLeft = std::ranges::views::filter(pixelIds, [&pixelIdLookup](const auto& pixelId)
  // {
  //   auto leftPixelId = pixelId.Get(cell_id::relative_position::left);
  //   return pixelIdLookup.contains(leftPixelId) ? false : true;
  // });

  // std::set<cell_id> boundaryLeftLookup;
  // std::ranges::copy(boundaryLeft, std::inserter(boundaryLeftLookup, std::begin(boundaryLeftLookup)));

  auto firstPixelId = pixelIdLookup.find(cell_id { 0, 0 });
  auto firstPixelRect = pixelSize.CellRect(*firstPixelId);

  std::list<POINT_2F> points;
  points.emplace_back(static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.top));
  points.emplace_back(static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.top));
  points.emplace_back(static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.bottom));
  points.emplace_back(static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.bottom));

  // constexpr auto pixelPositions = std::array {
  //   cell_id::relative_position::above_left,
  //   cell_id::relative_position::above,
  //   cell_id::relative_position::above_right,
  //   cell_id::relative_position::right,
  //   cell_id::relative_position::below_right,
  //   cell_id::relative_position::below,
  //   cell_id::relative_position::below_left
  // };

  // LoadNext(*firstPixelId, pixelSize, pixelPositions, pixelIdLookup, std::back_inserter(points));

  // std::list<cell_id::relative_position> geometryPositions;
  // geometryPositions.emplace_back(cell_id::relative_position::below_right);

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}

auto pixel_geometry::LoadNext(cell_id firstPixelId, cell_size pixelSize, std::ranges::input_range auto &&pixelPositions, const std::set<cell_id>& pixelIdLookup, auto backInserter) -> void
{
  constexpr auto nextPixelPositionWhenAbove = std::array {
    cell_id::relative_position::left,
    cell_id::relative_position::above,
    cell_id::relative_position::right
  };

  constexpr auto nextPixelPositionWhenRight = std::array {
    cell_id::relative_position::above,
    cell_id::relative_position::right,
    cell_id::relative_position::below,
  };

  constexpr auto nextPixelPositionWhenBelow = std::array {
    cell_id::relative_position::right,
    cell_id::relative_position::below,
    cell_id::relative_position::left
  };

  constexpr auto nextPixelPositionWhenLeft = std::array {
    cell_id::relative_position::below,
    cell_id::relative_position::left,
    cell_id::relative_position::above
  };

  auto firstPixelRect = pixelSize.CellRect(firstPixelId);

  for( auto pixelPosition : pixelPositions )
  {
    switch( pixelPosition )
    {
      case cell_id::relative_position::above:
        {
          auto nextPixelId = firstPixelId.Get(cell_id::relative_position::above);

          backInserter = POINT_2F { static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.top) };

          if( pixelIdLookup.contains(nextPixelId) )
          {
            // auto nextPixelRect = pixelSize.CellRect(nextPixelId);
            // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
            // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
            // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
            LoadNext(nextPixelId, pixelSize, nextPixelPositionWhenAbove, pixelIdLookup, backInserter);
          }

          break;
        }
        
      case cell_id::relative_position::right:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::right);
        
        backInserter = POINT_2F { static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.bottom) };

        if( pixelIdLookup.contains(nextPixelId) )
        {
          // auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
          LoadNext(nextPixelId, pixelSize, nextPixelPositionWhenRight, pixelIdLookup, backInserter);
        }

        break;
      }
        
      case cell_id::relative_position::below:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::below);

        backInserter = POINT_2F { static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.bottom) };

        if( pixelIdLookup.contains(nextPixelId) )
        {
          // auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
          LoadNext(nextPixelId, pixelSize, nextPixelPositionWhenBelow, pixelIdLookup, backInserter);
        }

        break;
      }
        
      case cell_id::relative_position::left:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::left);

        backInserter = POINT_2F { static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.top) };

        if( pixelIdLookup.contains(nextPixelId) )
        {
          // auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
          // backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
          LoadNext(nextPixelId, pixelSize, nextPixelPositionWhenLeft, pixelIdLookup, backInserter);
        }
      }
      break;
    }
  }
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
  }
}
