#ifndef _level_island_
#define _level_island_

#include "framework.h"
#include "screen_render_data.h"

struct level_island
{
  level_island(const game_closed_object& object);
  void RenderTo(ID2D1RenderTarget* renderTarget) const;

  game_closed_object object;
};

[[nodiscard]] inline auto HasCollided(const level_island& levelIsland, float x, float y) -> bool
{
  return PointInside(x, y, levelIsland.object);
}

inline auto HitByBullet(level_island&) -> void
{
}

inline auto GetRenderLines(const level_island&, render_line_inserter_type) -> void
{
}

inline auto LevelIsComplete(const level_island&) -> bool
{
  return true;
}

#endif
