#pragma once

#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class mine_renderer
{

public:

  auto Write(const enemy_bullet& object, ID2D1Geometry* geometry) const -> void
  {
    m_renderer.Write(geometry);
  }

private:

  geometry_renderer m_renderer { screen_render_brush_red.CreateBrush() };

};
