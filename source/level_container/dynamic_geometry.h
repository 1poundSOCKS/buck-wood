#pragma once

#include "framework.h"
#include "directional_body.h"

class dynamic_geometry
{

public:

  dynamic_geometry(std::ranges::input_range auto&& points);

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  auto Transform(const D2D1::Matrix3x2F& transform) -> void;

  operator const transformed_path_geometry&() const;

private:

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;

};

dynamic_geometry::dynamic_geometry(std::ranges::input_range auto&& points) : m_geometry { points }, m_transformedGeometry { points, D2D1::Matrix3x2F::Identity() }
{
}

inline [[nodiscard]] auto dynamic_geometry::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}

inline dynamic_geometry::operator const transformed_path_geometry&() const
{
  return m_transformedGeometry;
}
