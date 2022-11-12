#include "drag_drop.h"
#include <numeric>
#include "game_math.h"

void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
render_point::color GetBrushColor(const drag_drop_point& point);

drag_drop_point::drag_drop_point(float x, float y, type pointType)
: x(x), y(y), pointType(pointType)
{
}

drag_drop_line::drag_drop_line(const drag_drop_point& start, const drag_drop_point& end)
: start(start), end(end)
{
}

void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState)
{
  std::list<drag_drop_point>& boundaryPoints = dragDropState.boundary.points;
  drag_drop_shape& dragDropShape = dragDropState.dragDropShape;
  std::list<drag_drop_line>& dragDropLines = dragDropState.dragDropLines;
  std::list<drag_drop_point>& dragDropPoints = dragDropShape.points;
  
  dragDropState.dragDropLines.clear();
  CreateDragDropLines(boundaryPoints.cbegin(), boundaryPoints.cend(), std::back_inserter(dragDropLines));

  dragDropPoints.clear();
  CreateDragDropPoints(dragDropLines.cbegin(), dragDropLines.cend(), std::back_inserter(dragDropPoints));

  if( !controlState.leftMouseButtonDown )
  {
    HighlightClosestPoint(dragDropPoints.begin(), dragDropPoints.end(), controlState.worldMouseX, controlState.worldMouseY);
  }
  
  if( controlState.leftMouseButtonDown )
  {
    DragHighlightedPoints(dragDropPoints.begin(), dragDropPoints.end(), controlState.worldMouseX, controlState.worldMouseY);
  }

  boundaryPoints.clear();
  std::copy_if(dragDropPoints.cbegin(), dragDropPoints.cend(), std::back_inserter(boundaryPoints), [](const drag_drop_point& point)
  {
    return point.pointType == drag_drop_point::type::type_real;
  });
}

void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y)
{
  if( begin == end ) return;

  std::list<drag_drop_point>::iterator closestPoint = begin;
  begin->highlighted = false;
  float closestDistance = GetDistanceBetweenPoints(x, y, closestPoint->x, closestPoint->y);

  for( std::list<drag_drop_point>::iterator point = std::next(begin); point != end; point++ )
  {
    point->highlighted = false;
    float distance = GetDistanceBetweenPoints(x, y, point->x, point->y);
    if( distance < closestDistance ) closestPoint = point;
  }

  closestPoint->highlighted = true;
}

void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y)
{
  for( std::list<drag_drop_point>::iterator point = begin; point != end; ++point)
  {
    if( !point->highlighted ) continue;

    point->pointType = drag_drop_point::type::type_real;
    point->x = x;
    point->y = y;
  }
}

void CreateDragDropLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator)
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
  return drag_drop_point(line.start.x + cx / 2, line.start.y + cy / 2, drag_drop_point::type::type_virtual);
}

void TransformDragDropLinesIntoMiddlePoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_line& line)
  {
    return GetMiddlePoint(line);
  });
}

void CreateDragDropPoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  for( std::list<drag_drop_line>::const_iterator line = begin; line != end; ++line )
  {
    insertIterator = line->start;
    insertIterator = GetMiddlePoint(*line);
  }
}

void CreateRenderLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator)
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

// void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator)
// {
//   std::list<drag_drop_line> dragDropLines;
//   CreateDragDropLines(begin, end, std::back_inserter(dragDropLines));
//   CreateDragDropPoints(dragDropLines.cbegin(), dragDropLines.cend(), insertIterator);
// }

void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_point& dragDropPoint)
  {
    return render_point(dragDropPoint.x, dragDropPoint.y, 10, GetBrushColor(dragDropPoint));
  });
}

render_point::color GetBrushColor(const drag_drop_point& point)
{
  if( point.highlighted ) return render_point::color_red;

  switch( point.pointType )
  {
    case drag_drop_point::type::type_real:
      return render_point::color_green;
    default:
      return render_point::color_white;
  }
}
