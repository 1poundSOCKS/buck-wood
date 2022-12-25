#include "pch.h"
#include "level_state.h"
#include "game_objects.h"

void RenderGroundMatrix(
  ID2D1RenderTarget* renderTarget, 
  const screen_render_data& renderData,  
  const level_ground_matrix& groundMatrix) [[nothrow]];

void CreateDynamicLevelRenderLines(
  const level_state& levelState, 
  std::back_insert_iterator<std::vector<render_line>> renderLines, 
  screen_render_brush_selector brushes);

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

void RenderLevel(
  ID2D1RenderTarget* renderTarget, 
  const screen_render_data& renderData,
  const level_state& levelState)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->SetTransform(levelState.viewTransform);

  RenderGroundMatrix(renderTarget, renderData, levelState.groundMatrix);

  RenderPoints(renderTarget, levelState.renderStars.cbegin(), levelState.renderStars.cend());
  
  RenderLines(renderTarget, levelState.staticRenderLines.cbegin(), levelState.staticRenderLines.cend());

  std::vector<render_line> dynamicRenderLines;
  CreateDynamicLevelRenderLines(levelState, std::back_inserter(dynamicRenderLines), renderBrushSelector);
  auto renderTargetSize = renderTarget->GetSize();
  auto brush = renderBrushSelector[grey];
  AddGroundHorizontalRightHand(levelState, renderTargetSize, std::back_inserter(dynamicRenderLines), brush, 6);
  AddGroundHorizontalLeftHand(levelState, renderTargetSize, std::back_inserter(dynamicRenderLines), brush, 6);

  RenderLines(renderTarget, dynamicRenderLines.cbegin(), dynamicRenderLines.cend());

  std::vector<render_point> renderBullets;
  for( const auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;
    renderBullets.emplace_back(render_point { GetBulletRect(bullet.xPos, bullet.yPos), renderBrushSelector[red] });
  }

  RenderPoints(renderTarget, renderBullets.cbegin(), renderBullets.cend());
}

void RenderGroundMatrix(
  ID2D1RenderTarget* renderTarget, 
  const screen_render_data& renderData,  
  const level_ground_matrix& groundMatrix) [[nothrow]]
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  auto brush = renderBrushSelector[grey];

  std::vector<render_rect> renderRects;

  for( auto& row : groundMatrix.undergroundFlags )
  {
    std::vector<rect_underground_flag> rects;

    std::copy_if(
      row.cbegin(), row.cend(), std::back_inserter(rects), 
      [](auto& rect) -> bool { return rect.undergroundFlag; }
    );

    std::transform(
      rects.cbegin(),
      rects.cend(),
      std::back_inserter(renderRects),
      [&brush](auto& rect) -> render_rect { return { rect.rect.topLeft.x, rect.rect.topLeft.y, rect.rect.bottomRight.x, rect.rect.bottomRight.y, brush };  }
    );
  }

  RenderRectangles(renderTarget, renderRects.cbegin(), renderRects.cend());
}

void CreateDynamicLevelRenderLines(
  const level_state& levelState, 
  std::back_insert_iterator<std::vector<render_line>> renderLines, 
  screen_render_brush_selector brushes)
{
  for( const auto& target : levelState.targets )
  {
    auto renderBrush = target.activated ? brushes[red] : brushes[green];
    CreateRenderLines(target.shape.cbegin(), target.shape.cend(), renderLines, renderBrush, 4);
  }

  const auto& player = levelState.player;
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
