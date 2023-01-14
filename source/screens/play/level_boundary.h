#ifndef _level_boundary_
#define _level_boundary_

#include "geometry.h"
#include "game_level_data.h"

struct level_boundary
{
  game_open_object object;
};

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object;

[[nodiscard]] auto HasCollided(float x, float y, const level_boundary& levelBoundary) -> bool;

inline auto HitByBullet(level_boundary&) -> void
{
}

#endif
