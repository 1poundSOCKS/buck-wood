#ifndef _level_boundary_
#define _level_boundary_

#include "geometry.h"
#include "game_level_data.h"
#include "screen_render_data.h"

struct level_boundary
{
  void RenderTo(ID2D1RenderTarget* renderTarget) const;
  
  game_open_object object;
};

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object;

[[nodiscard]] auto HasCollided(const level_boundary& levelBoundary, float x, float y) -> bool;

inline auto HitByBullet(level_boundary&) -> void
{
}

inline auto GetRenderLines(const level_boundary&, render_line_inserter_type) -> void
{
}

inline auto LevelIsComplete(const level_boundary&) -> bool
{
  return true;
}

#endif
