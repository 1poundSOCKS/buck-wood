#include "drag_drop.h"
#include <numeric>
#include "game_math.h"

void InitializeDragDropShape(drag_drop_shape& shape);
void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState);
void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y, float proximity);
void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
void MarkHighlightedPointsAsDeleted(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end);
void CreateDragDropLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator);
void CreateDragDropPoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateRenderLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void CreateRenderLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator);
void CreateRenderPoint(const drag_drop_object& object, std::back_insert_iterator<std::vector<render_point>> insertIterator);
drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end);
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

void InitializeDragDrop(drag_drop_state& state)
{
  if( state.initialized ) return;

  for( auto& shape : state.shapes )
  {
    InitializeDragDropShape(shape);
  }

  state.initialized = true;
}

void InitializeDragDropShape(drag_drop_shape& shape)
{
  std::list<drag_drop_point> points;

  std::copy_if(shape.points.begin(), shape.points.end(), std::back_inserter(points), [](const auto& point)
  {
    return point.pointType == drag_drop_point::type::type_real;
  });

  shape.points.clear();

  for( auto point = points.begin(); point != points.end(); ++point )
  {
    auto nextPoint = std::next(point);
    if( nextPoint == points.end() ) nextPoint = points.begin();
    drag_drop_point middlePoint = GetMiddlePoint(*point, *nextPoint);
    shape.points.push_back(*point);
    shape.points.push_back(middlePoint);
  }
}

void ProcessDragDrop(drag_drop_state& state, const drag_drop_control_state& controlState)
{
  InitializeDragDrop(state);

  for( auto& shape : state.shapes )
  {
    ProcessDragDrop(shape, controlState);
  }

  if( controlState.leftMouseButtonReleased ) state.initialized = false;
}

void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState)
{
  std::list<drag_drop_point>& points = shape.points;

  if( !controlState.leftMouseButtonDown )
  {
    static const float proximity = 50;
    HighlightClosestPoint(points.begin(), points.end(), controlState.mouseX, controlState.mouseY, proximity);
  }
  
  if( controlState.leftMouseButtonDown )
  {
    DragHighlightedPoints(points.begin(), points.end(), controlState.mouseX, controlState.mouseY);
  }

  if( controlState.deleteItem )
  {
    MarkHighlightedPointsAsDeleted(points.begin(), points.end());
  }
}

void CreateRenderLines(std::vector<render_line>& lines, const drag_drop_state& state)
{
  for( const auto shape : state.shapes )
  {
    CreateRenderLines(shape.points.cbegin(), shape.points.cend(), std::back_inserter(lines));
  }
}

void CreateRenderLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator)
{
  std::transform(std::next(begin), end, begin, insertIterator, [](const auto& point1, const auto& point2)
  {
    D2D1_POINT_2F start, end;
    start.x = point2.x;
    start.y = point2.y;
    end.x = point1.x;
    end.y = point1.y;
    return render_line(start, end);
  });

  std::list<drag_drop_point>::const_iterator last = std::prev(end);

  D2D1_POINT_2F startPoint, endPoint;
  startPoint.x = last->x;
  startPoint.y = last->y;
  endPoint.x = begin->x;
  endPoint.y = begin->y;
  insertIterator = render_line(startPoint, endPoint);
}

void CreateRenderPoints(std::vector<render_point>& points, const drag_drop_state& state)
{
  for( const auto shape : state.shapes )
  {
    CreateRenderPoints(shape.points.cbegin(), shape.points.cend(), std::back_inserter(points));
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

drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end)
{
  float cx = end.x - start.x;
  float cy = end.y - start.y;
  return drag_drop_point(start.x + cx / 2, start.y + cy / 2, drag_drop_point::type::type_virtual);
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

void FormatDiagnostics(diagnostics_data& diagnosticsData, const drag_drop_state& state)
{
  static wchar_t text[64];

  for( const auto& shape : state.shapes )
  {
    for( const auto& point : shape.points )
    {
      if( point.highlighted )
      {
        swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", point.x, point.y, point.distance);
        diagnosticsData.push_back(text);
      }
    }
  }
}
