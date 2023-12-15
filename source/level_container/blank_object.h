#pragma once

#include "framework.h"

class blank_object
{

public:

  blank_object(std::ranges::input_range auto&& points);

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;

private:

  path_geometry m_geometry { d2d_factory::get_raw() };
  transformed_path_geometry m_transformedGeometry { m_geometry.Get(), D2D1::Matrix3x2F::Identity() };

};

inline blank_object::blank_object(std::ranges::input_range auto&& points)
{
  m_geometry.Load(points);
  m_transformedGeometry = transformed_path_geometry { d2d_factory::get_raw(), m_geometry.Get(), D2D1::IdentityMatrix() };
}

[[nodiscard]] inline auto blank_object::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
