#include "pch.h"
#include "level_geometry.h"
#include "framework.h"
#include "level_objects.h"

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]]
{
  decltype(level_ground_geometry::lines) lines;

  CreateConnectedLines(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(lines), 0, 0, false);

  for( auto& object : levelData.objects )
  {
    CreateConnectedLines(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
  }

  return {
    GetGameLevelBoundary(levelData),
    levelData.boundaryPoints.front(),
    levelData.boundaryPoints.back(),
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
  
  if( x >= levelGoundGeometry.groundStart.x && y > levelGoundGeometry.groundStart.y ) ++lineInterceptCount;
  if( x < levelGoundGeometry.groundEnd.x && y > levelGoundGeometry.groundEnd.y ) ++lineInterceptCount;

  return lineInterceptCount % 2 == 1;
}

[[nodiscard]] auto level_grid::CreateMatrix(const level_ground_geometry& groundGeometry)
-> std::vector<area_state> [[nothrow]]
{
  auto GetUndergroundState = [&groundGeometry](game_rect rect) -> area_state::state_type
  {
    auto cornerPoints = std::array {
      game_point { rect.topLeft.x, rect.topLeft.y }, 
      game_point { rect.bottomRight.x, rect.topLeft.y }, 
      game_point { rect.bottomRight.x, rect.bottomRight.y },
      game_point { rect.topLeft.x, rect.bottomRight.y }
    };

    auto IncrementIfUnderground = [&groundGeometry](auto count, auto point) -> int
    {
      return IsUnderground(point.x, point.y, groundGeometry) ? count + 1 : count;
    };

    auto undergroundCount = std::reduce(cornerPoints.cbegin(), cornerPoints.cend(), 0, IncrementIfUnderground);

    switch( undergroundCount )
    {
    case 4:
      return area_state::all_underground;
    case 0:
      // return area_state::not_underground;
    default:
      return area_state::part_underground;
    }
  };

  area_state levelAreaState = { groundGeometry.boundary, area_state::part_underground };

  std::vector<area_state> undergroundAreas;
  SplitMatrixPartials(levelAreaState, /*groundGeometry, */std::back_inserter(undergroundAreas), 9, GetUndergroundState);
  return undergroundAreas;
}
