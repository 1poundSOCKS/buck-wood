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

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry
{
  std::vector<game_point> points;
  std::copy(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(points));

  auto levelOuterPoints = GetOuterBoundaryPoints(levelData);
  std::copy(levelOuterPoints.cbegin(), levelOuterPoints.cend(), std::back_inserter(points));

  level_ground_geometry groundGeometry;
  groundGeometry.objects.emplace_back(LoadClosedObject(points.cbegin(), points.cend()));

  for( auto& object : levelData.objects )
  {
    groundGeometry.objects.emplace_back(LoadClosedObject(object.points.cbegin(), object.points.cend()));
  }

  for( auto& object : groundGeometry.objects )
  {
    std::copy(object.points.cbegin(), object.points.cend(), std::back_inserter(groundGeometry.allPoints));
    std::copy(object.lines.cbegin(), object.lines.cend(), std::back_inserter(groundGeometry.allLines));
  }

  return groundGeometry;
}

[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry
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

[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& groundGeometry) -> bool
{
  auto lineInterceptCount = std::accumulate(groundGeometry.objects.cbegin(), groundGeometry.objects.cend(), 0,
  [x, y, &groundGeometry](auto total, auto& object)
  {
    return total + GetLineInterceptCount({x, y}, object.lines.cbegin(), object.lines.cend());
  });

  return lineInterceptCount % 2 == 1;
}

[[nodiscard]] auto GetLevelBoundary(const level_ground_geometry& groundGeometry) -> game_rect
{
  std::vector<game_rect> boundaries;
  std::transform(groundGeometry.objects.cbegin(), groundGeometry.objects.end(), std::back_inserter(boundaries), 
  [](const game_closed_object& object) -> game_rect
  {
    return GetBoundingRect(object);
  });

  auto firstBoundary = boundaries.cbegin();
  return std::reduce(std::next(firstBoundary), boundaries.cend(), *firstBoundary,
  [](game_rect rect1, game_rect rect2) -> game_rect
  {
    return GetBoundingRect(rect1, rect2);
  });
}
