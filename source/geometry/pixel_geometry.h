#pragma once

#include "cell_id.h"
#include "cell_size.h"
#include "pixel_geometry_loader.h"

class pixel_geometry
{

public:

  pixel_geometry() noexcept = default;
  pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept;

  auto Load(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept -> void;

  [[nodiscard]] operator winrt::com_ptr<ID2D1Geometry>() const noexcept;

private:

  using pixel_id_lookup = std::set<cell_id>;

  auto LoadGeometry(const pixel_id_lookup& pixelIdLookup, cell_size pixelSize) noexcept -> void;

  static [[nodiscard]] auto NextPositionClockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position;
  static [[nodiscard]] auto NextPositionAnticlockwise(cell_id::relative_position currentPosition) -> cell_id::relative_position;
  static [[nodiscard]] auto OppositePosition(cell_id::relative_position currentPosition) -> cell_id::relative_position;
  static auto Bounds(std::ranges::input_range auto&& points) -> RECT_F;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};

pixel_geometry::pixel_geometry(std::ranges::input_range auto&& pixelIds, cell_size pixelSize) noexcept
{
  Load(pixelIds, pixelSize);
}

auto pixel_geometry::Load(std::ranges::input_range auto &&pixelIds, cell_size pixelSize) noexcept -> void
{
  // pixel_id_lookup pixelIdLookup;
  // std::ranges::copy(pixelIds, std::inserter(pixelIdLookup, std::begin(pixelIdLookup)));
  // LoadGeometry(pixelIdLookup, pixelSize);
  m_geometry = pixel_geometry_loader::read(pixelIds, pixelSize, '0');
}

auto pixel_geometry::Bounds(std::ranges::input_range auto &&points) -> RECT_F
{
  return std::accumulate(std::begin(points), std::end(points), RECT_F { 0.0f, 0.0f, 0.0f, 0.0f }, [](RECT_F bounds, POINT_2F point)
  {
    bounds.left = std::min(bounds.left, point.x);
    bounds.top = std::min(bounds.top, point.y);
    bounds.right = std::max(bounds.right, point.x);
    bounds.bottom = std::max(bounds.bottom, point.y);
    return bounds;
  });
}
