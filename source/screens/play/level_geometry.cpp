#include "pch.h"
#include "level_geometry.h"
#include "framework.h"
#include "level_objects.h"

[[nodiscard]] std::unique_ptr<level_geometry> CreateLevelGeometry(const game_level_data& levelData)
{
  std::unique_ptr<level_geometry> levelGeometry = std::make_unique<level_geometry>();
  levelGeometry->groundStart = levelData.boundaryPoints.front();
  levelGeometry->groundEnd = levelData.boundaryPoints.back();
  CreateConnectedLines(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(levelGeometry->lines), 0, 0, false);

  for( auto& object : levelData.objects )
  {
    CreateConnectedLines(object.points.cbegin(), object.points.cend(), std::back_inserter(levelGeometry->lines));
  }

  auto targetGeometry = GetDefaultTargetGeometryData();

  for( auto& target : levelData.targets )
  {
    CreateConnectedLines(targetGeometry.cbegin(), targetGeometry.cend(), std::back_inserter(levelGeometry->lines), target.x, target.y);
  }

  return levelGeometry;
}

[[nodiscard]] bool CoordinateIsVisible(float x, float y, const level_geometry* levelGeometry)
{
  auto lineInterceptCount = GetLineInterceptCount({x, y}, levelGeometry->lines.cbegin(), levelGeometry->lines.cend());
  
  if( x >= levelGeometry->groundStart.x && y > levelGeometry->groundStart.y ) ++lineInterceptCount;
  if( x < levelGeometry->groundEnd.x && y > levelGeometry->groundEnd.y ) ++lineInterceptCount;

  return lineInterceptCount % 2 == 0;
}
