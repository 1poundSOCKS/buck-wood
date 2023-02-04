#include "pch.h"
#include "level_island.h"

level_island::level_island(const game_closed_object& object) : object(object)
{
}

[[nodiscard]] auto level_island::HasCollided(float x, float y) const -> bool
{
  return PointInside(x, y, object);
}

auto level_island::HitByBullet() -> void
{
}

auto level_island::GetRenderLines(render_line_inserter_type) const -> void
{
}

[[nodiscard]] auto level_island::LevelIsComplete() const -> bool
{
  return true;
}

void level_island::RenderTo(ID2D1RenderTarget* renderTarget) const
{
  renderTarget;
}
