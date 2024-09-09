#pragma once

#include "cell_id.h"
#include "cell_size.h"

class pixel_geometry
{

public:

  pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size cellSize);
  [[nodiscard]] operator winrt::com_ptr<ID2D1Geometry>() const;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};

pixel_geometry::pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size cellSize)
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

  auto firstCellId = std::begin(pixelIdLookup);
  auto firstCellRect = cellSize.CellRect(*firstCellId);

  std::list<POINT_2F> points;
  points.emplace_back(static_cast<float>(firstCellRect.left), static_cast<float>(firstCellRect.top));
  points.emplace_back(static_cast<float>(firstCellRect.right), static_cast<float>(firstCellRect.top));
  points.emplace_back(static_cast<float>(firstCellRect.right), static_cast<float>(firstCellRect.bottom));
  points.emplace_back(static_cast<float>(firstCellRect.left), static_cast<float>(firstCellRect.bottom));

  // m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 5 }, D2D1_FIGURE_END_CLOSED);
  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}
