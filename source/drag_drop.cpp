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
    dragDropState.grabbedPoint = dragDropState.boundary.points.end();
    SelectClosestPoint(dragDropState.boundary, controlState.worldMouseX, controlState.worldMouseY);
    dragDropState.highlightedPoint = dragDropState.boundary.closestPoint;
  }

  bool drag = false;
  if( dragDropState.highlightedPoint != dragDropState.boundary.points.end() && controlState.leftMouseButtonDown )
  {
    dragDropState.grabbedPoint = dragDropState.highlightedPoint;
    dragDropState.highlightedPoint = dragDropState.boundary.points.end();
  }

  if( dragDropState.grabbedPoint != dragDropState.boundary.points.end() && controlState.leftMouseButtonDrag )
  {
    dragDropState.grabbedPoint->x = controlState.worldMouseX;
    dragDropState.grabbedPoint->y = controlState.worldMouseY;
  }
}

void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator)
{
  for( std::list<drag_drop_point>::const_iterator point = begin; point != end; ++point )
  {
    std::list<drag_drop_point>::const_iterator nextPointIt = std::next(point);
    const drag_drop_point& nextPoint = nextPointIt == end ? *begin : *nextPointIt;
    insertIterator = drag_drop_line(*point, nextPoint);
  }
}

drag_drop_point GetMiddlePoint(const drag_drop_line& line)
{
  float cx = line.end.x - line.start.x;
  float cy = line.end.y - line.start.y;
  return drag_drop_point(line.start.x + cx / 2, line.start.y + cy / 2);
}

void TransformDragDropLinesIntoMiddlePoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_line& line)
  {
    return GetMiddlePoint(line);
  });
}

void TransformDragDropLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_line& line)
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = line.start.x;
    startPoint.y = line.start.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = line.end.x;
    endPoint.y = line.end.y;

    return render_line(startPoint, endPoint);
  });
}

void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator)
{
  std::list<drag_drop_line> dragDropLines;
  TransformDragDropPoints(begin, end, std::back_inserter(dragDropLines));
  TransformDragDropLines(dragDropLines.cbegin(), dragDropLines.cend(), insertIterator);
}

void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_point& dragDropPoint)
  {
    return render_point(dragDropPoint.x, dragDropPoint.y, 10);
  });
}
