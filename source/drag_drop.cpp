#include "drag_drop.h"
#include <numeric>
#include "game_math.h"

void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState);
void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y, float proximity);
void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
void MarkHighlightedPointsAsDeleted(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end);
void CreateDragDropLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator);
void CreateDragDropPoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateRenderLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator);
void CreateRenderPoint(const drag_drop_object& object, std::back_insert_iterator<std::vector<render_point>> insertIterator);
render_point::color GetBrushColor(const drag_drop_point& point);

drag_drop_point::drag_drop_point(float x, float y, type pointType)
: x(x), y(y), pointType(pointType)
{
}

drag_drop_line::drag_drop_line(const drag_drop_point& start, const drag_drop_point& end)
: start(start), end(end)
{
}

drag_drop_object_point::drag_drop_object_point(float x, float y)
: x(x), y(y)
{
}

drag_drop_object::drag_drop_object()
: x(0), y(0)
{
}

drag_drop_object::drag_drop_object(float x, float y)
: x(x), y(y)
{
}

void ProcessDragDrop(drag_drop_state& state, const drag_drop_control_state& controlState)
{
  for( auto& shape : state.shapes )
  {
    ProcessDragDrop(shape, controlState);
  }
}

void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState)
{
  std::list<drag_drop_point>& points = shape.points;
  std::list<drag_drop_point>& dragDropPoints = shape.dragDropPoints;
  std::list<drag_drop_line>& lines = shape.lines;
  
  if( points.size() == 0 )
  {
    std::copy_if(dragDropPoints.cbegin(), dragDropPoints.cend(), std::back_inserter(points), [](const auto& point)
    {
      return (!point.deleted && point.pointType == drag_drop_point::type::type_real);
    });
    dragDropPoints.clear();
    lines.clear();
  }

  if( lines.size() == 0 )
  {
    lines.clear();
    CreateDragDropLines(points.cbegin(), points.cend(), std::back_inserter(lines));
  }

  if( dragDropPoints.size() == 0 )
  {
    dragDropPoints.clear();
    CreateDragDropPoints(lines.cbegin(), lines.cend(), std::back_inserter(dragDropPoints));
  }

  if( !controlState.leftMouseButtonDown )
  {
    static const float proximity = 50;
    HighlightClosestPoint(dragDropPoints.begin(), dragDropPoints.end(), controlState.mouseX, controlState.mouseY, proximity);
  }
  
  if( controlState.leftMouseButtonDown )
  {
    DragHighlightedPoints(dragDropPoints.begin(), dragDropPoints.end(), controlState.mouseX, controlState.mouseY);
    points.clear();
  }

  if( controlState.deleteItem )
  {
    MarkHighlightedPointsAsDeleted(dragDropPoints.begin(), dragDropPoints.end());
    points.clear();
  }
}

void CreateRenderLines(std::vector<render_line>& lines, const drag_drop_state& state)
{
  for( const auto shape : state.shapes )
  {
    CreateRenderLines(shape.lines.cbegin(), shape.lines.cend(), std::back_inserter(lines));
  }
}

void CreateRenderPoints(std::vector<render_point>& points, const drag_drop_state& state)
{
  for( const auto shape : state.shapes )
  {
    CreateRenderPoints(shape.dragDropPoints.cbegin(), shape.dragDropPoints.cend(), std::back_inserter(points));
  }
  for( const auto object : state.objects )
  {
    CreateRenderPoint(object, std::back_inserter(points));
  }
}

void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y, float proximity)
{
  for( auto point = begin; point != end; ++point )
  {
    point->highlighted = false;
    point->distance = GetDistanceBetweenPoints(x, y, point->x, point->y);
  }

  auto closestPoint = std::min_element(begin, end, [](const auto& first, const auto& second)
  {
    return first.distance < second.distance;
  });

  if( closestPoint != end && closestPoint->distance < proximity )
  {
    closestPoint->highlighted = true;
  }
}

void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y)
{
  for( auto point = begin; point != end; ++point)
  {
    if( point->highlighted )
    {
      point->pointType = drag_drop_point::type::type_real;
      point->x = x;
      point->y = y;
    }
  }
}

void MarkHighlightedPointsAsDeleted(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end)
{
  for( auto point = begin; point != end; ++point )
  {
    if( point->highlighted ) point->deleted = true;
  }
}

void CreateDragDropPoints(std::list<game_point>::const_iterator begin, std::list<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const game_point& point)
  {
    return drag_drop_point(point.x, point.y, drag_drop_point::type::type_real);
  });
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

void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_point& dragDropPoint)
  {
    return render_point(dragDropPoint.x, dragDropPoint.y, 10, GetBrushColor(dragDropPoint));
  });
}

void CreateRenderPoint(const drag_drop_object& object, std::back_insert_iterator<std::vector<render_point>> insertIterator)
{
  insertIterator = render_point(object.x, object.y, 50, render_point::color::color_white);
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
