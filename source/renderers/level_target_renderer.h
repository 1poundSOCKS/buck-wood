#pragma once

#include "framework.h"
#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class level_target_renderer
{

public:

  auto Write(const level_target& object, ID2D1Geometry* geometry) const -> void;

private:

  color_scale_brushes m_targetBrushes { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 10 } };

};

inline auto level_target_renderer::Write(const level_target& object, ID2D1Geometry* geometry) const -> void
{
  float halfAge = object.Age() / 2.0f;
  float cyclicAge = halfAge - static_cast<int>(halfAge);
  float fillBrushSelection = ( 1.0f - cyclicAge ) * 0.8f + 0.2f;
  geometry_renderer::Write(geometry, m_targetBrushes[fillBrushSelection].get(), m_targetBrushes[0.0f].get(), 10);
}
