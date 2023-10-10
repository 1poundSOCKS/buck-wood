#pragma once

#include "transformed_path_geometry.h"

class solid_object
{

public:

  solid_object(float left, float top, float right, float bottom);
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;

private:

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;

};

inline solid_object::solid_object(float left, float top, float right, float bottom)
{
  auto points = std::array
  {
    game_point { left, top },
    game_point { right, top },
    game_point { right, bottom },
    game_point { left, bottom }
  };

  m_geometry.Load(points);
  m_transformedGeometry = transformed_path_geometry { m_geometry, D2D1::IdentityMatrix() };
}

[[nodiscard]] inline auto solid_object::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
