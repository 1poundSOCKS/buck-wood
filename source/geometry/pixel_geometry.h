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

  {
    auto nextPixelId = firstPixelId->Get(cell_id::relative_position::above);

    if( pixelIdLookup.contains(nextPixelId) )
    {
      auto nextPixelRect = pixelSize.CellRect(nextPixelId);
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top));
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top));
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom));
    }
    else
    {
      points.emplace_back(static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.top));
    } 
  }

  {
    auto nextPixelId = firstPixelId->Get(cell_id::relative_position::right);
    
    if( pixelIdLookup.contains(nextPixelId) )
    {
      auto nextPixelRect = pixelSize.CellRect(nextPixelId);
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top));
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom));
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom));
    }
    else
    {
      points.emplace_back(static_cast<float>(firstPixelRect.right), static_cast<float>(firstPixelRect.bottom));
    }
  }

  {
    auto nextPixelId = firstPixelId->Get(cell_id::relative_position::below);

    if( pixelIdLookup.contains(nextPixelId) )
    {
      auto nextPixelRect = pixelSize.CellRect(nextPixelId);
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.bottom));
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom));
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top));
    }
    else
    {
      points.emplace_back(static_cast<float>(firstPixelRect.left), static_cast<float>(firstPixelRect.bottom));
    }
  }

  {
    auto nextPixelId = firstPixelId->Get(cell_id::relative_position::left);

    if( pixelIdLookup.contains(nextPixelId) )
    {
      auto nextPixelRect = pixelSize.CellRect(nextPixelId);
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.bottom));
      points.emplace_back(static_cast<float>(nextPixelRect.left), static_cast<float>(nextPixelRect.top));
      points.emplace_back(static_cast<float>(nextPixelRect.right), static_cast<float>(nextPixelRect.top));
    }
  }

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}
