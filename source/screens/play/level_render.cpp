#include "pch.h"
#include "level_state.h"
#include "game_objects.h"

void RenderGround(ID2D1RenderTarget* renderTarget, const screen_render_data& renderData,  const level_state& levelState);

void CreateDynamicLevelRenderLines(
  const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines, screen_render_brush_selector brushes);

void AddGroundHorizontalRightHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, auto renderLinesInserter, ID2D1SolidColorBrush* brush, float width)
{
  D2D1_POINT_2F inPoint { renderTargetSize.width, 0 };
  auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);

  renderLinesInserter = { 
    { levelState.levelData.boundaryPoints.front().x, levelState.levelData.boundaryPoints.front().y} , 
    { outPoint.x, levelState.levelData.boundaryPoints.front().y },
    brush, 
    width
  };
}

void AddGroundHorizontalLeftHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, auto renderLinesInserter, ID2D1SolidColorBrush* brush, float width)
{
  D2D1_POINT_2F inPoint { 0, 0 };
  auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);

  renderLinesInserter = { 
    { levelState.levelData.boundaryPoints.back().x, levelState.levelData.boundaryPoints.back().y} , 
    { outPoint.x, levelState.levelData.boundaryPoints.back().y },
    brush, width
  };
}

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -4, -4, 4, 4 };
}

inline D2D1_RECT_F GetBulletRect(float x, float y)
{
  const D2D1_RECT_F rect = GetBulletRect(); 
  return { rect.left + x, rect.top + y, rect.right + x, rect.bottom + y };
}

auto RenderLevel(
  ID2D1RenderTarget* renderTarget, 
  const screen_render_data& renderData,
  const level_state& levelState) -> void
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->SetTransform(levelState.viewTransform);

  RenderGround(renderTarget, renderData, levelState);

  RenderPoints(renderTarget, levelState.renderStars.cbegin(), levelState.renderStars.cend());

#ifdef __RENDER_GROUND_LINES
  RenderLines(renderTarget, levelState.staticRenderLines.cbegin(), levelState.staticRenderLines.cend());
#endif

  std::vector<render_line> dynamicRenderLines;
  CreateDynamicLevelRenderLines(levelState, std::back_inserter(dynamicRenderLines), renderBrushSelector);
  auto renderTargetSize = renderTarget->GetSize();

#ifdef __RENDER_GROUND_LINES  
  auto brush = renderBrushSelector[grey];
  AddGroundHorizontalRightHand(levelState, renderTargetSize, std::back_inserter(dynamicRenderLines), brush, 6);
  AddGroundHorizontalLeftHand(levelState, renderTargetSize, std::back_inserter(dynamicRenderLines), brush, 6);
#endif

  RenderLines(renderTarget, dynamicRenderLines.cbegin(), dynamicRenderLines.cend());

  std::vector<render_point> renderBullets;
  for( const auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;
    renderBullets.emplace_back(render_point { GetBulletRect(bullet.xPos, bullet.yPos), renderBrushSelector[red] });
  }

  RenderPoints(renderTarget, renderBullets.cbegin(), renderBullets.cend());

  std::vector<render_point> explosionParticles;
  CreateRenderPoints(levelState.explosions.cbegin(), levelState.explosions.cend(), renderData.renderBrushes, std::back_inserter(explosionParticles));
  RenderPoints(renderTarget, explosionParticles.cbegin(), explosionParticles.cend());
}

void RenderGround(
  ID2D1RenderTarget* renderTarget, 
  const screen_render_data& renderData,
  const level_state& levelState)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  auto greyBrush = renderBrushSelector[grey];
  auto darkGreyBrush = renderBrushSelector[dark_grey];

  std::vector<area_state> visibleCells;
  std::copy_if(levelState.groundMatrix.cbegin(), levelState.groundMatrix.cend(), std::back_inserter(visibleCells), 
  [&levelState](area_state areaState) -> bool
  {
    return DoOverlap(areaState.rect, levelState.viewRect);
  });

  auto ConvertAreaStateToRenderRect = [&greyBrush, &darkGreyBrush](auto& areaState) -> render_rect
  {
    auto& brush = ( areaState.state == area_state::split ? darkGreyBrush : greyBrush );

    return {
      areaState.rect.topLeft.x, 
      areaState.rect.topLeft.y, 
      areaState.rect.bottomRight.x + 1, 
      areaState.rect.bottomRight.y + 1, 
      brush
    };
  };

  std::vector<render_rect> renderRects;
  std::transform(visibleCells.cbegin(), visibleCells.cend(), std::back_inserter(renderRects), ConvertAreaStateToRenderRect);

  RenderRectangles(renderTarget, renderRects.cbegin(), renderRects.cend());
}

void CreateDynamicLevelRenderLines(
  const level_state& levelState, 
  std::back_insert_iterator<std::vector<render_line>> renderLines, 
  screen_render_brush_selector brushes)
{
  // for( const auto& target : levelState.targets )
  // {
  //   auto renderBrush = target.activated ? brushes[red] : brushes[green];
  //   CreateRenderLines(target.shape.cbegin(), target.shape.cend(), renderLines, renderBrush, 4);
  // }

  for( const auto& object : levelState.solidObjects )
  {
    object.GetRenderLines(renderLines);
  }

  if( levelState.player.state == player_ship::alive )
  {
    CreateConnectedRenderLines(
      levelState.player.points.cbegin(), 
      levelState.player.points.cend(), 
      renderLines, 
      brushes[white], 
      2);

    if( levelState.player.thrusterOn )
    {
      std::vector<game_point> thrusterPoints;
      GetTransformedThrusterGeometry(levelState.player, std::back_inserter(thrusterPoints));

      CreateDisconnectedRenderLines(
        thrusterPoints.cbegin(), 
        thrusterPoints.cend(), 
        renderLines, brushes[red], 5);
    }
  }
}
