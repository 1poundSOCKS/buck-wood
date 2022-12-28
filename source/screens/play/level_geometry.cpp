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

[[nodiscard]] auto level_grid::GetDefinition(int columnWidth, int rowHeight, const level_ground_geometry& groundGeometry) -> level_grid::definition [[nothrow]]
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

[[nodiscard]] auto level_grid::GetDefinition(int scaleFactor, const level_ground_geometry& groundGeometry) -> definition [[nothrow]]
{
  int cellSize = pow(2,scaleFactor);
  return level_grid::GetDefinition(cellSize, cellSize, groundGeometry);
}

[[nodiscard]] auto level_grid::CreateCellReferences(const definition& gridDef) -> std::vector<cell_ref> [[nothrow]]
{
  std::vector<int> rows(gridDef.rowCount);
  std::iota(rows.begin(), rows.end(), gridDef.firstRow);

  std::vector<int> columns(gridDef.columnCount);
  std::iota(columns.begin(), columns.end(), gridDef.firstColumn);

  std::vector<cell_ref> allCells;
  auto newCell = std::back_inserter(allCells);

  for( auto row : rows )
  {
    for( auto column : columns )
    {
      newCell = { column, row };
    }
  }

  return allCells;
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

[[nodiscard]] auto level_grid::CreateGrid(level_grid::definition gridDef, const std::vector<cell_ref>& cellRefs)
-> std::vector<game_rect> [[nothrow]]
{
  std::vector<game_rect> allRects;

  std::transform(cellRefs.cbegin(), cellRefs.cend(), std::back_inserter(allRects), 
  [&gridDef](auto cellRef) -> game_rect
  {
    return GetGridRect(cellRef.column, cellRef.row, gridDef.columnWidth, gridDef.rowHeight);
  });

  return allRects;
}

[[nodiscard]] auto level_grid::GetUndergroundState(game_rect rect, const level_ground_geometry& groundGeometry)
-> area_state::state_type [[nothrow]]
{
  std::array<game_point, 4> cornerPoints = {
    rect.topLeft.x, rect.topLeft.y, 
    rect.bottomRight.x, rect.topLeft.y, 
    rect.bottomRight.x, rect.bottomRight.y,
    rect.topLeft.x, rect.bottomRight.y
  };

  auto IsPointUnderground = [&groundGeometry](auto point)
  {
    return IsUnderground(point.x, point.y, groundGeometry);
  };

  std::vector<game_point> undergroundPoints;
  std::copy_if(cornerPoints.cbegin(), cornerPoints.cend(), std::back_inserter(undergroundPoints), IsPointUnderground);

  switch( undergroundPoints.size() )
  {
  case 4:
    return area_state::all_underground;
  case 0:
    return area_state::not_underground;
  default:
    return area_state::part_underground;
  }
}

[[nodiscard]] auto level_grid::CreateMatrix(const std::vector<game_rect>& grid, const level_ground_geometry& groundGeometry)
-> std::vector<area_state> [[nothrow]]
{
  std::vector<area_state> allRects;

  auto ConvertRectToAreaState = [&groundGeometry](auto rect) -> area_state
  {
    return { rect, GetUndergroundState(rect, groundGeometry) };
  };

  std::transform(grid.cbegin(), grid.cend(), std::back_inserter(allRects), ConvertRectToAreaState);

  return allRects;
}

[[nodiscard]] auto level_grid::SplitMatrixPartials(const std::vector<area_state>& matrix, const level_ground_geometry& groundGeometry)
-> std::vector<area_state> [[nothrow]]
{
  auto IsAllUnderground = [](auto area) -> bool
  {
    return area.state == area_state::all_underground;
  };

  auto IsPartUnderground = [](auto area) -> bool
  {
    return area.state == area_state::part_underground;
  };

  std::vector<area_state> allUnderground;
  std::copy_if(matrix.cbegin(), matrix.cend(), std::back_inserter(allUnderground), IsAllUnderground);

  std::vector<area_state> partUnderground;
  std::copy_if(matrix.cbegin(), matrix.cend(), std::back_inserter(partUnderground), IsPartUnderground);

  for( auto area : partUnderground )
  {
    SplitMatrixPartials(area, groundGeometry, std::back_inserter(allUnderground), 4);
  }

  return allUnderground;
}
