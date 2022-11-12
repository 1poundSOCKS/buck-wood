#ifndef _drag_drop_
#define _drag_drop_

#include <list>
#include <memory>
#include <limits>
#include <d2d1.h>
#include "render.h"

struct drag_drop_point
{
  enum type { type_real, type_virtual };

  drag_drop_point(float x, float y, type pointType);

  float x;
  float y;
  type pointType;
  bool highlighted = false;
  float distance = 0;
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
  std::list<drag_drop_point> dragDropPoints;
  std::list<drag_drop_line> dragDropLines;
  // drag_drop_shape::iterator highlightedPoint = boundary.points.end();
  // drag_drop_shape::iterator grabbedPoint = boundary.points.end();
};

struct drag_drop_control_state
{
  bool leftMouseButtonDown = false;
  bool leftMouseButtonDrag = false;
  float mouseX = 0;
  float mouseY = 0;
};

void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState);

void CreateDragDropPoints(std::list<game_point>::const_iterator begin, std::list<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateDragDropLines(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_line>> insertIterator);
void CreateDragDropPoints(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateRenderLines(std::list<drag_drop_line>::const_iterator begin, std::list<drag_drop_line>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator);
void CreateRenderPoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<render_point>> insertIterator);

#endif
