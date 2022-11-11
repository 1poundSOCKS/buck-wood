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
  drag_drop_shape boundary;
  drag_drop_shape::iterator highlightedPoint = boundary.points.end();
  drag_drop_shape::iterator grabbedPoint = boundary.points.end();
};

struct drag_drop_control_state
{
  bool leftMouseButtonDown = false;
  bool leftMouseButtonDrag = false;
  float worldMouseX = 0;
  float worldMouseY = 0;
};

template<class T> void DragDropTransform(std::list<game_point>::const_iterator begin, std::list<game_point>::const_iterator end, T insertIterator)
{
  std::transform(begin, end, insertIterator, [](const game_point& point)
  {
    return drag_drop_point(point.x, point.y);
  });
}

void SelectClosestPoint(drag_drop_shape& shape, float x, float y);
void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState);

void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator);
void TransformDragDropLinesIntoMiddlePoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void TransformDragDropLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void TransformDragDropPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator);

#endif
