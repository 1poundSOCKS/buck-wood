#pragma once

#include "framework.h"

class dynamic_geometry
{

public:

  dynamic_geometry(std::ranges::input_range auto&& points, const D2D1::Matrix3x2F& transform);
  dynamic_geometry(std::ranges::input_range auto&& points);

  dynamic_geometry(const path_geometry& pathGeometry, const D2D1::Matrix3x2F& transform);
  dynamic_geometry(const path_geometry& pathGeometry);

  [[nodiscard]] auto Get() const -> ID2D1TransformedGeometry*;

  auto Transform(const D2D1::Matrix3x2F& transform) -> void;

  operator const transformed_path_geometry&() const;

  [[nodiscard]] auto HasCollidedWith(const transformed_path_geometry& geometry) const -> bool;
  [[nodiscard]] auto HasCollidedWith(const game_point& point) const -> bool;

private:

  transformed_path_geometry m_transformedGeometry;

};

dynamic_geometry::dynamic_geometry(std::ranges::input_range auto&& points, const D2D1::Matrix3x2F& transform) : m_transformedGeometry { points, transform }
{
}

dynamic_geometry::dynamic_geometry(std::ranges::input_range auto&& points) : dynamic_geometry(points, D2D1::Matrix3x2F::Identity())
{
}

inline [[nodiscard]] auto dynamic_geometry::Get() const -> ID2D1TransformedGeometry*
{
  return m_transformedGeometry.Get();
}

inline dynamic_geometry::operator const transformed_path_geometry&() const
{
  return m_transformedGeometry;
}

inline [[nodiscard]] auto dynamic_geometry::HasCollidedWith(const transformed_path_geometry& geometry) const -> bool
{
  return m_transformedGeometry.HasCollidedWith(geometry);
}

inline [[nodiscard]] auto dynamic_geometry::HasCollidedWith(const game_point& point) const -> bool
{
  return m_transformedGeometry.HasCollidedWith(point);
}
