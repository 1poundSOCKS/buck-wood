#pragma once

#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class mine_renderer
{

public:

  auto Write(const mine& object, ID2D1Geometry* geometry) const -> void
  {
    switch( object.Type() )
    {
      case mine::type::one:
        m_rendererOne.Write(geometry);
        return;
      case mine::type::two:
        m_rendererTwo.Write(geometry);
        return;
    }
  }

private:

  geometry_renderer m_rendererOne { screen_render_brush_type_one.CreateBrush() };
  geometry_renderer m_rendererTwo { screen_render_brush_type_two.CreateBrush() };

};
