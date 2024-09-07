#pragma once

#include "cell_id.h"

class pixel_geometry
{

public:

  pixel_geometry(std::ranges::input_range auto&& pixelIds);
  [[nodiscard]] operator winrt::com_ptr<ID2D1Geometry>() const;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};

pixel_geometry::pixel_geometry(std::ranges::input_range auto&& pixelIds)
{
  std::set<cell_id> pixelIdLookup;
  std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));

  auto boundaryLeft = std::ranges::views::filter(pixelIds, [&pixelIdLookup](const auto& pixelId)
  {
    auto leftPixelId = pixelId.Get(cell_id::relative_position::left);
    return pixelIdLookup.contains(leftPixelId) ? false : true;
  });

  std::set<cell_id> boundaryLeftLookup;
  std::ranges::copy(boundaryLeft, std::inserter(boundaryLeftLookup, std::begin(boundaryLeftLookup)));

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 5 }, D2D1_FIGURE_END_CLOSED);
}
