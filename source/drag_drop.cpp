#include "pch.h"
#include "drag_drop.h"
// #include <numeric>
#include "game_math.h"

void InitializeDragDropShape(drag_drop_shape& shape);
void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState);
void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y, float proximity);
void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
void MarkHighlightedPointsAsDeleted(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end);
void CreateRenderLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator, float x=0, float y=0);
void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator);
render_point CreateRenderPoint(const drag_drop_point& point);
drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end);
render_brushes::color GetBrushColor(const drag_drop_point& point);

drag_drop_point::drag_drop_point(float x, float y, type pointType)
: x(x), y(y), pointType(pointType)
{
}

drag_drop_shape::drag_drop_shape(int type, float x, float y)
: type(type), position(x, y)
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
  static const float proximity = 50;
  
  auto& points = shape.points;

  if( controlState.leftMouseButtonDown )
  {
    if( shape.fixedShape )
    {
      if( shape.position.highlighted )
      {
        shape.position.x = controlState.mouseX;
        shape.position.y = controlState.mouseY;
      }
    }
    else
    {
      DragHighlightedPoints(points.begin(), points.end(), controlState.mouseX, controlState.mouseY);
    }
  }
  else
  {
    if( shape.fixedShape )
    {
      float distance = GetDistanceBetweenPoints(shape.position.x, shape.position.y, controlState.mouseX, controlState.mouseY);
      shape.position.highlighted = distance < proximity;
    }
    else
    {
      HighlightClosestPoint(points.begin(), points.end(), controlState.mouseX, controlState.mouseY, proximity);
    }
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
    CreateRenderLines(shape.points.cbegin(), shape.points.cend(), std::back_inserter(lines), shape.position.x, shape.position.y);
  }
}

void CreateRenderLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator, float x, float y)
{
  std::transform(std::next(begin), end, begin, insertIterator, [x,y](const auto& point1, const auto& point2)
  {
    D2D1_POINT_2F start, end;
    start.x = point2.x + x;
    start.y = point2.y + y;
    end.x = point1.x + x;
    end.y = point1.y + y;
    return render_line(start, end);
  });

  std::list<drag_drop_point>::const_iterator last = std::prev(end);

  D2D1_POINT_2F startPoint, endPoint;
  startPoint.x = last->x + x;
  startPoint.y = last->y + y;
  endPoint.x = begin->x + x;
  endPoint.y = begin->y + y;
  insertIterator = render_line(startPoint, endPoint);
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

drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end)
{
  float cx = end.x - start.x;
  float cy = end.y - start.y;
  return drag_drop_point(start.x + cx / 2, start.y + cy / 2, drag_drop_point::type::type_virtual);
}

void CreateRenderPoints(std::vector<render_point>& points, const drag_drop_state& state)
{
  for( const auto shape : state.shapes )
  {
    if( shape.fixedShape ) points.push_back(CreateRenderPoint(shape.position));
    else CreateRenderPoints(shape.points.cbegin(), shape.points.cend(), std::back_inserter(points));
  }
}

void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const drag_drop_point& dragDropPoint)
  {
    return CreateRenderPoint(dragDropPoint);
  });
}

render_point CreateRenderPoint(const drag_drop_point& point)
{
  return render_point(point.x, point.y, 10, GetBrushColor(point));
}

render_brushes::color GetBrushColor(const drag_drop_point& point)
{
  if( point.highlighted ) return render_brushes::color::color_red;

  switch( point.pointType )
  {
    case drag_drop_point::type::type_undefined:
    case drag_drop_point::type::type_real:
      return render_brushes::color::color_green;
    default:
      return render_brushes::color::color_white;
  }
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const drag_drop_state& state)
{
  static wchar_t text[64];

  for( const auto& shape : state.shapes )
  {
    if( shape.fixedShape && shape.position.highlighted )
    {
      swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", shape.position.x, shape.position.y, shape.position.distance);
      diagnosticsData.push_back(text);
    }

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
