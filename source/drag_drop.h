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
  bool deleted = false;
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
};

struct drag_drop_object_point
{
  drag_drop_object_point(float x, float y);

  float x, y;
};

struct drag_drop_object
{
  drag_drop_object();
  drag_drop_object(float x, float y);

  float x, y;
  bool highlighted = false;

  // std::list<drag_drop_object_point> points;
};

struct drag_drop_state
{
  bool initialized = false;
  std::vector<drag_drop_shape> shapes;
  std::vector<drag_drop_object> objects;
};

struct drag_drop_control_state
{
  bool leftMouseButtonDown = false;
  bool leftMouseButtonDrag = false;
  bool leftMouseButtonReleased = false;
  float mouseX = 0;
  float mouseY = 0;
  bool deleteItem = false;
};

void InitializeDragDrop(drag_drop_state& state);
void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState);
void CreateRenderLines(std::vector<render_line>& lines, const drag_drop_state& state);
void CreateRenderPoints(std::vector<render_point>& points, const drag_drop_state& state);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const drag_drop_state& state);

#endif
