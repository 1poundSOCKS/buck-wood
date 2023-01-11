#ifndef _level_boundary_
#define _level_boundary_

#include "geometry.h"

struct level_boundary
{
  game_closed_object object;
};

[[nodiscard]] auto HasCollided(float x, float y, const level_boundary& levelBoundary) -> bool;

#endif
