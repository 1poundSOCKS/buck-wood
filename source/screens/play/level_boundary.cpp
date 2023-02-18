#include "pch.h"
#include "level_boundary.h"
#include "collisions.h"

level_boundary::level_boundary(const game_open_object& object, screen_render_brush_selector brushes) : object(object)
{
  brush.attach(brushes[brown]);
  brush->AddRef();
}

auto level_boundary::Update(int64_t tickFrequency, int64_t tickCount) -> void
{
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

void level_boundary::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const
{
  std::vector<render_line> renderLines;
  auto renderLinesInserter = std::back_inserter(renderLines);
  CreateRenderLines(object.lines.cbegin(), object.lines.cend(), renderLinesInserter, brush.get(), 6);

  renderLinesInserter = { 
    { object.points.front().x, object.points.front().y} , 
    { viewRect.right, object.points.front().y },
    brush.get(), 6
  };

  renderLinesInserter = { 
    { object.points.back().x, object.points.back().y} , 
    { viewRect.left, object.points.back().y },
    brush.get(), 6
  };

  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object
{
  return LoadOpenObject(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend());
}
