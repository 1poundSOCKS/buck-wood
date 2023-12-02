#pragma once

#include "transformed_path_geometry.h"

class solid_object
{

public:

  solid_object(float left, float top, float right, float bottom);
  solid_object(std::ranges::input_range auto&& points);

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;

private:

  path_geometry m_geometry { d2d_factory::get_raw() };
  transformed_path_geometry m_transformedGeometry;

};

inline solid_object::solid_object(float left, float top, float right, float bottom)
{
  auto points = std::array
  {
    game_point { left, top },
    game_point { right + 1, top },
    game_point { right + 1, bottom + 1 },
    game_point { left, bottom + 1 }
  };

  m_geometry.Load(points);
  m_transformedGeometry = transformed_path_geometry { d2d_factory::get_raw(), m_geometry.Get(), D2D1::IdentityMatrix() };
}

inline solid_object::solid_object(std::ranges::input_range auto&& points)
{
  m_geometry.Load(points);
  m_transformedGeometry = transformed_path_geometry { d2d_factory::get_raw(), m_geometry.Get(), D2D1::IdentityMatrix() };
}

[[nodiscard]] inline auto solid_object::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
