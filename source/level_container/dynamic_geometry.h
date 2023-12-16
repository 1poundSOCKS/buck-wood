#pragma once

#include "framework.h"
#include "directional_body.h"

class dynamic_geometry
{

public:

  dynamic_geometry(std::ranges::input_range auto&& points, const D2D1::Matrix3x2F& transform);
  dynamic_geometry(std::ranges::input_range auto&& points);

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto Get() const -> ID2D1TransformedGeometry*;

  auto Transform(const D2D1::Matrix3x2F& transform) -> void;

  operator const transformed_path_geometry&() const;

private:

  transformed_path_geometry m_transformedGeometry;

};

dynamic_geometry::dynamic_geometry(std::ranges::input_range auto&& points, const D2D1::Matrix3x2F& transform) : m_transformedGeometry { points, transform }
{
}

dynamic_geometry::dynamic_geometry(std::ranges::input_range auto&& points) : dynamic_geometry(points, D2D1::Matrix3x2F::Identity())
{
}

inline [[nodiscard]] auto dynamic_geometry::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}

inline [[nodiscard]] auto dynamic_geometry::Get() const -> ID2D1TransformedGeometry*
{
  return m_transformedGeometry.Get();
}

inline dynamic_geometry::operator const transformed_path_geometry&() const
{
  return m_transformedGeometry;
}
