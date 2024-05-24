#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class player_ship_renderer
{

public:

  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
  {
    m_renderer.Write(geometry);
  }

private:

  geometry_renderer m_renderer { screen_render_brush_type_one.CreateBrush() };

};
