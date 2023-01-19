#include "pch.h"
#include "level_boundary.h"
#include "collisions.h"

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object
{
  return LoadOpenObject(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend());
}

[[nodiscard]] auto HasCollided(const level_boundary& levelBoundary, float x, float y) -> bool
{
  auto lineCount = GetLineInterceptCount({ x, y }, levelBoundary.object.lines.cbegin(), levelBoundary.object.lines.cend());
  auto firstPoint = levelBoundary.object.points.front();
  auto lastPoint = levelBoundary.object.points.back();
  if( x > firstPoint.x && y > firstPoint.y ) ++lineCount;
  if( x <= lastPoint.x && y > lastPoint.y ) ++lineCount;
  return ( lineCount % 2 == 1 ) ? true : false;
}
