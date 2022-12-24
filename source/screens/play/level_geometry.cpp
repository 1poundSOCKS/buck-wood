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

[[nodiscard]] auto IsUnderground(game_rect rect, const level_ground_geometry& groundGeometry) -> bool [[nothrow]]
{
  return
    IsUnderground(rect.topLeft.x, rect.topLeft.y, groundGeometry) && 
    IsUnderground(rect.bottomRight.x, rect.topLeft.y, groundGeometry) && 
    IsUnderground(rect.bottomRight.x, rect.bottomRight.y, groundGeometry) &&
    IsUnderground(rect.topLeft.x, rect.bottomRight.y, groundGeometry);
}

[[nodiscard]] auto CreateLevelGridDef(int columnWidth, int rowHeight, const level_ground_geometry& groundGeometry) -> level_grid_def [[nothrow]]
{
  int firstColumn = static_cast<int>(groundGeometry.boundary.topLeft.x) / columnWidth;
  int lastColumn = static_cast<int>(groundGeometry.boundary.bottomRight.x) / columnWidth;
  
  int firstRow = static_cast<int>(groundGeometry.boundary.topLeft.y) / rowHeight;
  int lastRow = static_cast<int>(groundGeometry.boundary.bottomRight.y) / rowHeight;

  game_rect topLeft = {
    static_cast<float>(firstColumn * columnWidth),
    static_cast<float>(firstRow * rowHeight)
  };

  return {
    columnWidth,
    rowHeight,
    firstColumn,
    lastColumn - firstColumn + 1,
    firstRow,
    lastRow - firstRow + 1
  };
}

[[nodiscard]] auto GetGridRect(int row, int column, int columnWidth, int rowHeight) -> game_rect [[nothrow]]
{
  return {
    static_cast<float>(row) * columnWidth,
    static_cast<float>(column) * rowHeight,
    ( static_cast<float>(row) + 1 ) * columnWidth,
    ( static_cast<float>(column) + 1 ) * rowHeight
  };
}

[[nodiscard]] auto CreateLevelRectGrid(level_grid_def gridDef) -> level_rect_grid [[nothrow]]
{
  std::vector<int> rows(gridDef.rowCount);
  std::iota(rows.begin(), rows.end(), gridDef.firstRow);

  std::vector<std::vector<game_rect>> allRects;
  std::transform(rows.cbegin(), rows.cend(), std::back_inserter(allRects),
  [&gridDef](auto row) -> std::vector<game_rect>
  {
    std::vector<int> columns(gridDef.columnCount);
    std::iota(columns.begin(), columns.end(), gridDef.firstColumn);
    
    std::vector<game_rect> rowRects;
    std::transform(columns.cbegin(), columns.cend(), std::back_inserter(rowRects), 
    [row, &gridDef](auto column) -> game_rect
    {
      return GetGridRect(row, column, gridDef.columnWidth, gridDef.rowHeight);
    });

    return rowRects;
  });

  return { allRects };
}

[[nodiscard]] auto CreateLevelGroundMatrix(int granularity, const level_ground_geometry& groundGeometry) -> level_ground_matrix [[nothrow]]
{
  std::vector<std::vector<rect_underground_flag>> undergroundFlags;
  return { undergroundFlags };
}
