#pragma once

#include "geometry.h"
#include "transformed_path_geometry.h"

class level_asteroid
{
public:

  level_asteroid(float x, float y, float maxWidth, float maxHeight);
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;

private:

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};
