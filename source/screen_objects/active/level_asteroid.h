#pragma once

#include "object_input_data.h"
#include "geometry.h"
#include "path_geometry.h"

class level_asteroid
{
public:

  level_asteroid(game_rect rect);
  [[nodiscard]] auto Geometry() const -> const path_geometry&;

private:

  path_geometry m_geometry;
};
