#pragma once

#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"
#include "path_geometry.h"

class level_asteroid
{
public:

  level_asteroid(const game_closed_object& object);
  level_asteroid(game_rect rect);
  [[nodiscard]] auto Geometry() const -> const path_geometry&;

private:

  game_closed_object m_object;
  path_geometry m_geometry;
};
