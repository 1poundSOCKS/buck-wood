#include "pch.h"
#include "level_state.h"
#include "game_objects.h"

void CreateStaticLevelRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> insertIterator, screen_render_brush_selector brushes);
void CreateDynamicLevelRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines, screen_render_brush_selector brushes);

template <typename insert_iterator_type>
void AddGroundHorizontalRightHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, insert_iterator_type renderLines, ID2D1SolidColorBrush* brush, float width);

template <typename insert_iterator_type>
void AddGroundHorizontalLeftHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, insert_iterator_type renderLines, ID2D1SolidColorBrush* brush, float width);

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
  const bespoke_render_data& renderData,
  const level_state& levelState)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->SetTransform(levelState.viewTransform);

  RenderPoints(renderTarget, levelState.renderStars.cbegin(), levelState.renderStars.cend());

  std::vector<render_line> renderLines;
  CreateStaticLevelRenderLines(levelState, std::back_inserter(renderLines), renderBrushSelector);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());

  renderLines.clear();
  CreateDynamicLevelRenderLines(levelState, std::back_inserter(renderLines), renderBrushSelector);

  auto renderTargetSize = renderTarget->GetSize();
  auto brush = renderBrushSelector[grey];
  AddGroundHorizontalRightHand(levelState, renderTargetSize, std::back_inserter(renderLines), brush, 6);
  AddGroundHorizontalLeftHand(levelState, renderTargetSize, std::back_inserter(renderLines), brush, 6);

  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());

  std::vector<render_point> renderBullets;
  for( const auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;
    renderBullets.emplace_back(render_point { GetBulletRect(bullet.xPos, bullet.yPos), renderBrushSelector[red] });
  }

  RenderPoints(renderTarget, renderBullets.cbegin(), renderBullets.cend());
}

void CreateStaticLevelRenderLines(
  const level_state& levelState,
  std::back_insert_iterator<std::vector<render_line>> insertIterator, 
  screen_render_brush_selector brushes)
{
  auto brush = brushes[grey];
  CreateRenderLines(levelState.groundLines.cbegin(), levelState.groundLines.cend(), insertIterator, brush,6);
  CreateRenderLines(levelState.objectLines.cbegin(), levelState.objectLines.cend(), insertIterator, brush,6);
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
    levelState.player.transformedPoints.cbegin(), 
    levelState.player.transformedPoints.cend(), 
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

template <typename insert_iterator_type>
void AddGroundHorizontalRightHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, insert_iterator_type renderLines, ID2D1SolidColorBrush* brush, float width)
{
  D2D1_POINT_2F inPoint { renderTargetSize.width, 0 };
  auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);

  renderLines = { 
    { levelState.levelData.boundaryPoints.front().x, levelState.levelData.boundaryPoints.front().y} , 
    { outPoint.x, levelState.levelData.boundaryPoints.front().y },
    brush, 
    width
  };
}

template <typename insert_iterator_type>
void AddGroundHorizontalLeftHand(const level_state& levelState, D2D1_SIZE_F renderTargetSize, insert_iterator_type renderLines, ID2D1SolidColorBrush* brush, float width)
{
  D2D1_POINT_2F inPoint { 0, 0 };
  auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);

  renderLines = { 
    { levelState.levelData.boundaryPoints.back().x, levelState.levelData.boundaryPoints.back().y} , 
    { outPoint.x, levelState.levelData.boundaryPoints.back().y },
    brush, width
  };
}
