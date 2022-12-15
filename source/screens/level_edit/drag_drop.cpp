#include "pch.h"
#include "drag_drop.h"
#include "geometry.h"

void InitializeDragDropShape(drag_drop_shape& shape);
void ProcessDragDrop(drag_drop_shape& shape, const drag_drop_control_state& controlState);
void HighlightClosestPoint(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y, float proximity);
void DragHighlightedPoints(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end, float x, float y);
void MarkHighlightedPointsAsDeleted(std::list<drag_drop_point>::iterator begin, std::list<drag_drop_point>::iterator end);
void CreateDragDropRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator, screen_render_brush_selector renderBrushSelector);
drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end);
ID2D1SolidColorBrush *GetDragDropPointBrush(const drag_drop_point& point, screen_render_brush_selector renderBrushSelector);

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

    if( nextPoint == points.end() )
    nextPoint = points.begin();

    shape.points.push_back(*point);

    if( shape.closed || nextPoint != points.begin() )
    {
      drag_drop_point middlePoint = GetMiddlePoint(*point, *nextPoint);
      shape.points.push_back(middlePoint);
    }
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

void CreateDragDropRenderLines(std::vector<render_line>& lines, const drag_drop_state& state, screen_render_brush_selector renderBrushSelector)
{
  for( const auto shape : state.shapes )
  {
    CreateConnectedRenderLines(shape.points.cbegin(), shape.points.cend(), std::back_inserter(lines), renderBrushSelector[white], 2, shape.position.x, shape.position.y);
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

drag_drop_point GetMiddlePoint(const drag_drop_point& start, const drag_drop_point& end)
{
  float cx = end.x - start.x;
  float cy = end.y - start.y;
  return drag_drop_point(start.x + cx / 2, start.y + cy / 2, drag_drop_point::type::type_virtual);
}

void CreateDragDropRenderPoints(
  std::vector<render_point>& points, 
  const drag_drop_state& state, 
  screen_render_brush_selector renderBrushSelector)
{
  for( const auto shape : state.shapes )
  {
    if( shape.fixedShape ) points.push_back(render_point(shape.position.x, shape.position.y, GetDragDropPointBrush(shape.position, renderBrushSelector), 4));
    else CreateDragDropRenderPoints(shape.points.cbegin(), shape.points.cend(), std::back_inserter(points), renderBrushSelector);
  }
}

void CreateDragDropRenderPoints(
  std::list<drag_drop_point>::const_iterator begin, 
  std::list<drag_drop_point>::const_iterator end, 
  std::back_insert_iterator<std::vector<render_point>> insertIterator, 
  screen_render_brush_selector renderBrushSelector)
{
  std::transform(begin, end, insertIterator, [renderBrushSelector](const drag_drop_point& dragDropPoint)
  {
    return render_point(dragDropPoint.x, dragDropPoint.y, GetDragDropPointBrush(dragDropPoint, renderBrushSelector), 8);
  });
}

ID2D1SolidColorBrush* GetDragDropPointBrush(const drag_drop_point& point, screen_render_brush_selector renderBrushSelector)
{
  if( point.highlighted ) return renderBrushSelector[red];
  else
  {
    switch( point.pointType )
    {
      case drag_drop_point::type::type_undefined:
      case drag_drop_point::type::type_real:
        return renderBrushSelector[green];
      default:
        return renderBrushSelector[white];
    }
  }
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const drag_drop_state& state)
{
  static wchar_t text[64];

  for( const auto& shape : state.shapes )
  {
    if( shape.fixedShape && shape.position.highlighted )
    {
      swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", shape.position.x, shape.position.y, shape.position.distance);
      diagnosticsData = text;
    }

    for( const auto& point : shape.points )
    {
      if( point.highlighted )
      {
        swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", point.x, point.y, point.distance);
        diagnosticsData = text;
      }
    }
  }
}
