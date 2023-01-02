#include "pch.h"
#include "level_geometry.h"
#include "framework.h"
#include "level_objects.h"

[[nodiscard]] auto GetOuterBoundaryPoints(const game_level_data& levelData) -> std::array<game_point, 4>
{
  auto levelBoundary = GetGameLevelBoundary(levelData);

  const auto boundaryExtension = 600.0f;

  auto outerLevelBoundary = game_rect {
    game_point { levelBoundary.topLeft.x - boundaryExtension, levelBoundary.topLeft.y }, 
    game_point { levelBoundary.bottomRight.x + boundaryExtension, levelBoundary.bottomRight.y + boundaryExtension }
  };

  auto outerStartPoint = levelData.boundaryPoints.front();
  auto outerEndPoint = levelData.boundaryPoints.back();

  return std::array {
    game_point { outerLevelBoundary.topLeft.x, outerEndPoint.y },
    game_point { outerLevelBoundary.topLeft.x, outerLevelBoundary.bottomRight.y },
    game_point { outerLevelBoundary.bottomRight.x, outerLevelBoundary.bottomRight.y },
    game_point { outerLevelBoundary.bottomRight.x, outerStartPoint.y }
  };
}

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]]
{
  decltype(level_ground_geometry::points) points;
  std::copy(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(points));

  auto levelOuterPoints = GetOuterBoundaryPoints(levelData);
  std::copy(levelOuterPoints.cbegin(), levelOuterPoints.cend(), std::back_inserter(points));

  decltype(level_ground_geometry::lines) lines;
  CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(lines));

  for( auto& object : levelData.objects )
  {
    std::copy(object.points.cbegin(), object.points.cend(), std::back_inserter(points));
    CreateConnectedLines(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
  }

  return {
    points,
    lines
  };
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
  
  // if( x >= levelGoundGeometry.groundStart.x && y > levelGoundGeometry.groundStart.y ) ++lineInterceptCount;
  // if( x < levelGoundGeometry.groundEnd.x && y > levelGoundGeometry.groundEnd.y ) ++lineInterceptCount;

  return lineInterceptCount % 2 == 1;
}

[[nodiscard]] auto GetLevelBoundary(const level_ground_geometry& groundGeometry) -> game_rect [[nothrow]]
{
  auto GetBoundaryRect = [&groundGeometry](auto rect, auto point) -> game_rect
  {
    return {
      min(rect.topLeft.x, point.x),
      min(rect.topLeft.y, point.y),
      max(rect.bottomRight.x, point.x),
      max(rect.bottomRight.y, point.y)
    };
  };

  return std::reduce(groundGeometry.points.cbegin(), groundGeometry.points.cend(), game_rect { 0, 0, 0, 0 }, GetBoundaryRect);
}
