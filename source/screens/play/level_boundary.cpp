#include "pch.h"
#include "level_boundary.h"
#include "collisions.h"

[[nodiscard]] auto level_boundary::HasCollided(float x, float y) const -> bool
{
  auto lineCount = GetLineInterceptCount({ x, y }, object.lines.cbegin(), object.lines.cend());
  auto firstPoint = object.points.front();
  auto lastPoint = object.points.back();
  if( x > firstPoint.x && y > firstPoint.y ) ++lineCount;
  if( x <= lastPoint.x && y > lastPoint.y ) ++lineCount;
  return ( lineCount % 2 == 1 ) ? true : false;
}

auto level_boundary::HitByBullet() -> void
{
}

auto level_boundary::GetRenderLines(render_line_inserter_type) const -> void
{
}

[[nodiscard]] auto level_boundary::LevelIsComplete() const -> bool
{
  return true;
}

void level_boundary::RenderTo(ID2D1RenderTarget*) const
{
}

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object
{
  return LoadOpenObject(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend());
}
