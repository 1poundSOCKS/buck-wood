#include "pch.h"
#include "level_state.h"
#include "game_objects.h"

void CreateStaticLevelRenderLines(const game_level_data& gameLevelData, std::back_insert_iterator<std::vector<render_line>> insertIterator, screen_render_brush_selector brushes);
void CreateDynamicLevelRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines, screen_render_brush_selector brushes);

void RenderLevel(
  ID2D1RenderTarget* renderTarget, 
  const bespoke_render_data& renderData,
  const level_state& levelState,
  float renderScale)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  auto renderTargetSize = renderTarget->GetSize();
  renderTarget->SetTransform(CreateViewTransform(levelState, renderTargetSize, renderScale));

  std::vector<render_line> staticRenderLines;
  CreateStaticLevelRenderLines(levelState.levelData, std::back_inserter(staticRenderLines), renderBrushSelector);

  RenderLines(renderTarget, staticRenderLines.cbegin(), staticRenderLines.cend());

  std::vector<render_line> renderLines;
  CreateDynamicLevelRenderLines(levelState, std::back_inserter(renderLines), renderBrushSelector);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());

  std::vector<render_point> renderBullets;
  for( const auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;
    renderBullets.emplace_back(render_point(bullet.xPos, bullet.yPos, renderBrushSelector[red], 8));
  }

  RenderPoints(renderTarget, renderBullets.cbegin(), renderBullets.cend());
}

void CreateStaticLevelRenderLines(
  const game_level_data& gameLevelData, 
  std::back_insert_iterator<std::vector<render_line>> insertIterator, 
  screen_render_brush_selector brushes)
{
  CreateConnectedRenderLines(
    gameLevelData.boundaryPoints.cbegin(), 
    gameLevelData.boundaryPoints.cend(), 
    insertIterator, brushes[grey], 
    6, 0, 0, false
  );

  const game_point& firstBoundaryPoint = gameLevelData.boundaryPoints.front();
  
  insertIterator = render_line
  {
    D2D1_POINT_2F { firstBoundaryPoint.x, firstBoundaryPoint.y }, 
    D2D1_POINT_2F { firstBoundaryPoint.x + 1000, firstBoundaryPoint.y }, 
    brushes[grey],
    6
  };

  const game_point& lastBoundaryPoint = gameLevelData.boundaryPoints.back();

  insertIterator = render_line
  {
    D2D1_POINT_2F { lastBoundaryPoint.x, lastBoundaryPoint.y }, 
    D2D1_POINT_2F { lastBoundaryPoint.x - 1000, lastBoundaryPoint.y }, 
    brushes[grey],
    6
  };

  for( auto& object : gameLevelData.objects )
  {
    CreateConnectedRenderLines(object.points.cbegin(), object.points.cend(), insertIterator, brushes[grey], 6);
  }
}

void CreateDynamicLevelRenderLines(
  const level_state& levelState, 
  std::back_insert_iterator<std::vector<render_line>> renderLines, 
  screen_render_brush_selector brushes)
{
  for( const auto& target : levelState.targets )
  {
    auto renderBrush = target.activated ? brushes[red] : brushes[green];
    std::vector<game_point> points;
    CreatePointsForTarget(defaultTargetSize, std::back_inserter(points));
    std::vector<game_point> transformedPoints;
    TransformPoints(points.cbegin(), points.cend(), std::back_inserter(transformedPoints), 0, target.position.x, target.position.y);
    CreateConnectedRenderLines(transformedPoints.cbegin(), transformedPoints.cend(), renderLines, renderBrush, 4);
  }

  const auto& player = levelState.player;
  CreateConnectedRenderLines(
    levelState.playerShipPointData.transformedPoints.cbegin(), 
    levelState.playerShipPointData.transformedPoints.cend(), 
    renderLines, 
    brushes[white], 
    2);

  if( levelState.player.thrusterOn )
  {
    std::vector<game_point> points;
    points.reserve(levelState.playerShipPointData.thrusterPoints.size());
    
    TransformPoints(
      levelState.playerShipPointData.thrusterPoints.cbegin(), 
      levelState.playerShipPointData.thrusterPoints.cend(), 
      std::back_inserter(points), 
      player.angle, 
      player.xPos, 
      player.yPos);

    CreateDisconnectedRenderLines(points.cbegin(), points.cend(), renderLines, brushes[red], 5);
  }
}
