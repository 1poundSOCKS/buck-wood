#ifndef _drag_drop_
#define _drag_drop_

// #include <list>
// #include <memory>
// #include <limits>
// #include <d2d1.h>
#include "render.h"
#include "diagnostics.h"

struct drag_drop_point
{
  enum type { type_undefined, type_real, type_virtual };

  drag_drop_point(float x=0, float y=0, type pointType=type_undefined);

  float x;
  float y;
  type pointType;
  bool highlighted = false;
  bool deleted = false;
  float distance = 0;
};

struct drag_drop_shape
{
  drag_drop_shape(int type, float x=0, float y=0);

  int type;
  bool fixedShape = false;
  drag_drop_point position;
  std::list<drag_drop_point> points;
};

struct drag_drop_object_point
{
  drag_drop_object_point(float x, float y);

  float x, y;
};

struct drag_drop_state
{
  bool initialized = false;
  std::vector<drag_drop_shape> shapes;
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
