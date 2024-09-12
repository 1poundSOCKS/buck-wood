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

  constexpr auto pixelPositions = std::array {
    cell_id::relative_position::above,
    cell_id::relative_position::right,
    cell_id::relative_position::below,
    cell_id::relative_position::left
  };

  LoadNext(*firstPixelId, pixelSize, pixelPositions, pixelIdLookup, std::back_inserter(points));

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}

auto pixel_geometry::LoadNext(cell_id firstPixelId, cell_size pixelSize, std::ranges::input_range auto &&pixelPositions, const std::set<cell_id>& pixelIdLookup, auto backInserter) -> void
{
  auto firstPixelRect = pixelSize.CellRect(firstPixelId);

  for( auto pixelPosition : pixelPositions )
  {
    switch( pixelPosition )
    {
      case cell_id::relative_position::above:
        {
          auto nextPixelId = firstPixelId.Get(cell_id::relative_position::above);

          if( pixelIdLookup.contains(nextPixelId) )
          {
            auto nextPixelRect = pixelSize.CellRect(nextPixelId);
            backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
            backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
            backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
          }
          else
          {
            backInserter = POINT_2F { static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.top) };
          }
          break;
        }
        
      case cell_id::relative_position::right:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::right);
        
        if( pixelIdLookup.contains(nextPixelId) )
        {
          auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
        }
        else
        {
          backInserter = POINT_2F { static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.bottom) };
        }
        break;
      }
        
      case cell_id::relative_position::below:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::below);

        if( pixelIdLookup.contains(nextPixelId) )
        {
          auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
        }
        else
        {
          backInserter = POINT_2F { static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.bottom) };
        }
        break;
      }
        
      case cell_id::relative_position::left:
      {
        auto nextPixelId = firstPixelId.Get(cell_id::relative_position::left);

        if( pixelIdLookup.contains(nextPixelId) )
        {
          auto nextPixelRect = pixelSize.CellRect(nextPixelId);
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top) };
          backInserter = POINT_2F { static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top) };
        }
      }
      break;
    }
  }
}
