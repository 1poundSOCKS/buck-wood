#pragma once

#include "transformed_path_geometry.h"

class solid_object
{

public:

  solid_object();
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;

private:

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;

};

inline solid_object::solid_object()
{
  constexpr auto points = std::array
  {
    game_point { 1000, 1000 },
    game_point { 2000, 1000 },
    game_point { 2000, 2000 },
    game_point { 1000, 2000 }
  };

  m_geometry.Load(points);
  m_transformedGeometry = transformed_path_geometry { m_geometry, D2D1::IdentityMatrix() };
}

[[nodiscard]] inline auto solid_object::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
