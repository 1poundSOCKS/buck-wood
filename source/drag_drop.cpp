#include "drag_drop.h"
#include <numeric>
#include "game_math.h"

drag_drop_point::drag_drop_point(float x, float y) : x(x), y(y)
{
}

drag_drop_line::drag_drop_line(const drag_drop_point& start, const drag_drop_point& end)
: start(start), end(end)
{
}

void SelectClosestPoint(drag_drop_shape& shape, float x, float y)
{
  drag_drop_shape::iterator currentPoint = shape.closestPoint = shape.points.begin();
  
  if( shape.points.size() == 0 ) return;

  float closestDistance = GetDistanceBetweenPoints(x, y, shape.closestPoint->x, shape.closestPoint->y);
  
  while( ++currentPoint != shape.points.end() )
  {
    float currentDistance = GetDistanceBetweenPoints(x, y, currentPoint->x, currentPoint->y);
    if( currentDistance < closestDistance )
    {
      shape.closestPoint = currentPoint;
      closestDistance = currentDistance;      
    }
  }
}

void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState)
{
  if( !controlState.leftMouseButtonDown )
  {
    dragDropState.grabbedPoint = dragDropState.shape.points.end();
    SelectClosestPoint(dragDropState.shape, controlState.worldMouseX, controlState.worldMouseY);
    dragDropState.highlightedPoint = dragDropState.shape.closestPoint;
  }

  bool drag = false;
  if( dragDropState.highlightedPoint != dragDropState.shape.points.end() && controlState.leftMouseButtonDown )
  {
    dragDropState.grabbedPoint = dragDropState.highlightedPoint;
    dragDropState.highlightedPoint = dragDropState.shape.points.end();
  }

  if( dragDropState.grabbedPoint != dragDropState.shape.points.end() && controlState.leftMouseButtonDrag )
  {
    dragDropState.grabbedPoint->x = controlState.worldMouseX;
    dragDropState.grabbedPoint->y = controlState.worldMouseY;
  }
}

void CreateDragDropRenderLines(std::list<render_line>& lines, const std::list<drag_drop_point>& points)
{
  for( std::list<drag_drop_point>::const_iterator iStart = points.cbegin(); iStart != points.cend(); iStart++ )
  {
    std::list<drag_drop_point>::const_iterator iEnd = std::next(iStart);
    if( iEnd == points.cend() ) iEnd = points.cbegin();

    D2D1_POINT_2F startPoint;
    startPoint.x = iStart->x;
    startPoint.y = iStart->y;

    D2D1_POINT_2F endPoint;
    endPoint.x = iEnd->x;
    endPoint.y = iEnd->y;

    lines.push_back(render_line(startPoint, endPoint));
  }
}

void CreateDragDropRenderPoints(std::list<render_point>& renderPoints, const std::list<drag_drop_point>& points)
{
  std::transform(points.cbegin(), points.cend(), std::back_inserter(renderPoints), [](const drag_drop_point& point)
  {
    return render_point(point.x, point.y, 10);
  });
}

void RenderDragDrop(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const drag_drop_state& dragDropState, const d2d_brushes& brushes)
{
  RenderDragDropShape(renderTarget, dragDropState.shape, brushes);
}

void RenderDragDropShape(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const drag_drop_shape& shape, const d2d_brushes& brushes)
{
  std::list<render_line> lines;
  CreateDragDropRenderLines(lines, shape.points);

  for( const auto& line : lines )
  {
    RenderLine(renderTarget, line, 5, brushes.brush);
  }

  std::list<render_point> renderPoints;
  CreateDragDropRenderPoints(renderPoints, shape.points);
  
  for( const auto& renderPoint : renderPoints )
  {
    RenderPoint(renderTarget, renderPoint, brushes.brushDeactivated);
  }

  render_point renderPoint(shape.closestPoint->x, shape.closestPoint->y, 10);
  if( shape.closestPoint != shape.points.end() )
  {
    RenderPoint(renderTarget, renderPoint, brushes.brushActivated);
  }
}
