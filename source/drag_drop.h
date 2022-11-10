#ifndef _drag_drop_
#define _drag_drop_

#include <list>
#include <memory>
#include <d2d1.h>
#include "render.h"

struct drag_drop_point
{
  drag_drop_point(float x, float y);

  float x;
  float y;
};

struct drag_drop_line
{
  drag_drop_line(const drag_drop_point& start, const drag_drop_point& end);
  
  drag_drop_point start, end;
};

struct drag_drop_shape
{
  std::list<drag_drop_point> points;
  using iterator = std::list<drag_drop_point>::iterator;
  iterator closestPoint = points.end();
};

struct drag_drop_state
{
  drag_drop_shape shape;
  drag_drop_shape::iterator highlightedPoint = shape.points.end();
  drag_drop_shape::iterator grabbedPoint = shape.points.end();
};

struct drag_drop_control_state
{
  bool leftMouseButtonDown = false;
  bool leftMouseButtonDrag = false;
  float worldMouseX = 0;
  float worldMouseY = 0;
};

void SelectClosestPoint(drag_drop_shape& shape, float x, float y);
void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState);
void CreateDragDropRenderLines(std::list<render_line>& lines, const std::list<drag_drop_point>& points);
void CreateDragDropRenderPoints(std::list<render_point>& renderPoints, const std::list<drag_drop_point>& points);
void RenderDragDrop(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const drag_drop_state& dragDropState, const d2d_brushes& brushes);
void RenderDragDropShape(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const drag_drop_shape& shape, const d2d_brushes& brushes);

#endif
