#include "pch.h"
#include "level_geometry.h"
#include "framework.h"
#include "level_objects.h"

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]]
{
  level_ground_geometry levelGoundGeometry;
  levelGoundGeometry.groundStart = levelData.boundaryPoints.front();
  levelGoundGeometry.groundEnd = levelData.boundaryPoints.back();
  CreateConnectedLines(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(levelGoundGeometry.lines), 0, 0, false);

  for( auto& object : levelData.objects )
  {
    CreateConnectedLines(object.points.cbegin(), object.points.cend(), std::back_inserter(levelGoundGeometry.lines));
  }

  return levelGoundGeometry;
}

[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry [[nothrow]]
{
  level_targets_geometry levelTargetsGeometry;
  
  auto targetGeometry = GetDefaultTargetGeometryData();

  for( auto& target : levelData.targets )
  {
    std::vector<game_point> transformedPoints;

    TransformPoints(
      targetGeometry.cbegin(), 
      targetGeometry.cend(), 
      std::back_inserter(transformedPoints), 
      D2D1::Matrix3x2F::Translation(target.x, target.y)
    );

    std::copy(
      transformedPoints.cbegin(), 
      transformedPoints.cend(),
      std::back_inserter(levelTargetsGeometry.points)
    );
    
    CreateConnectedLines(
      transformedPoints.cbegin(), 
      transformedPoints.cend(), 
      std::back_inserter(levelTargetsGeometry.lines)
    );
  }

  return levelTargetsGeometry;
}

[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGoundGeometry) -> bool [[nothrow]]
{
  auto lineInterceptCount = GetLineInterceptCount({x, y}, levelGoundGeometry.lines.cbegin(), levelGoundGeometry.lines.cend());
  
  if( x >= levelGoundGeometry.groundStart.x && y > levelGoundGeometry.groundStart.y ) ++lineInterceptCount;
  if( x < levelGoundGeometry.groundEnd.x && y > levelGoundGeometry.groundEnd.y ) ++lineInterceptCount;

  return lineInterceptCount % 2 == 1;
}

[[nodiscard]] auto IsUnderground(game_rect rect, const level_ground_geometry& groundGeometry) -> bool [[nothrow]]
{
  return
    IsUnderground(rect.topLeft.x, rect.topLeft.y, groundGeometry) && 
    IsUnderground(rect.bottomRight.x, rect.bottomRight.y, groundGeometry);
}
