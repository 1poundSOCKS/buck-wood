#ifndef _level_island_
#define _level_island_

#include "framework.h"
#include "screen_render_data.h"

struct level_island
{
  level_island(const game_closed_object& object);

  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  auto GetRenderLines(render_line_inserter_type inserter) const -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  void RenderTo(ID2D1RenderTarget* renderTarget) const;

  game_closed_object object;
};

#endif
