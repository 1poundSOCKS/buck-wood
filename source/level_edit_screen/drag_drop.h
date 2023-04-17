#ifndef _drag_drop_
#define _drag_drop_

#include "render.h"
#include "diagnostics.h"
#include "screen_render_data.h"

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

  bool closed = true;
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

void CreateDragDropRenderLines(std::vector<render_line>& lines, const drag_drop_state& state, screen_render_brush_selector renderBrushSelector);

void CreateDragDropRenderPoints(std::vector<render_point>& points, const drag_drop_state& state, screen_render_brush_selector renderBrushSelector);

void FormatDiagnostics(const drag_drop_state& state, auto diagnosticsDataInserter)
{
  static wchar_t text[64];

  for( const auto& shape : state.shapes )
  {
    if( shape.fixedShape && shape.position.highlighted )
    {
      swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", shape.position.x, shape.position.y, shape.position.distance);
      diagnosticsDataInserter = text;
    }

    for( const auto& point : shape.points )
    {
      if( point.highlighted )
      {
        swprintf(text, L"highlight point: %.1f, %.1f (%.1f)", point.x, point.y, point.distance);
        diagnosticsDataInserter = text;
      }
    }
  }
}

#endif
