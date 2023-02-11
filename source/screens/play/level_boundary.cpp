#include "pch.h"
#include "level_boundary.h"
#include "collisions.h"

level_boundary::level_boundary(const game_open_object& object, screen_render_brush_selector brushes) : object(object)
{
  brush.attach(brushes[brown]);
  brush->AddRef();
}

[[nodiscard]] auto level_boundary::HasCollided(float x, float y) const -> bool
{
  auto lineCount = GetLineInterceptCount({ x, y }, object.lines.cbegin(), object.lines.cend());
  auto firstPoint = object.points.front();
  auto lastPoint = object.points.back();
  if( x > firstPoint.x && y > firstPoint.y ) ++lineCount;
  if( x <= lastPoint.x && y > lastPoint.y ) ++lineCount;
  return ( lineCount % 2 == 1 ) ? true : false;
}

auto level_boundary::HitByBullet() -> void
{
}

[[nodiscard]] auto level_boundary::LevelIsComplete() const -> bool
{
  return true;
}

void level_boundary::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F) const
{
  std::vector<render_line> renderLines;
  CreateRenderLines(object.lines.cbegin(), object.lines.cend(), std::back_inserter(renderLines), brush.get(), 6);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object
{
  return LoadOpenObject(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend());
}

void level_boundary::AddGroundHorizontalRightHand(D2D1_SIZE_F renderTargetSize, const D2D1::Matrix3x2F& invertedViewTransform, auto renderLinesInserter)
{
  D2D1_POINT_2F inPoint { renderTargetSize.width, 0 };
  auto outPoint = invertedViewTransform.TransformPoint(inPoint);

  renderLinesInserter = { 
    { object.points.front().x, object.points.front().y} , 
    { outPoint.x, object.points.front().y },
    brush.get(), 6
  };
}

void level_boundary::AddGroundHorizontalLeftHand(D2D1_SIZE_F renderTargetSize, const D2D1::Matrix3x2F& invertedViewTransform, auto renderLinesInserter)
{
  D2D1_POINT_2F inPoint { 0, 0 };
  auto outPoint = invertedViewTransform.TransformPoint(inPoint);

  renderLinesInserter = { 
    { object.points.back().x, object.points.back().y} , 
    { outPoint.x, object.points.back().y },
    brush.get(), 6
  };
}
