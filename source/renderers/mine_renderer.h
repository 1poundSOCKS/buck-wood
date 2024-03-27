#pragma once

#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class mine_renderer
{

public:

  auto Write(const enemy_bullet_1& object, ID2D1Geometry* geometry) const -> void
  {
    switch( object.Type() )
    {
      case enemy_bullet_1::type::one:
        m_rendererOne.Write(geometry);
        return;
      case enemy_bullet_1::type::two:
        m_rendererTwo.Write(geometry);
        return;
    }
  }

private:

  geometry_renderer m_rendererOne { screen_render_brush_type_one.CreateBrush() };
  geometry_renderer m_rendererTwo { screen_render_brush_type_two.CreateBrush() };

};
