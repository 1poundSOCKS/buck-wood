#pragma once

#include "level_data_types.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class mine_renderer
{

public:

  auto Write(const mine_object& object) const -> void
  {
    switch( object->HardnessType() )
    {
      case mine::hardness_type::soft:
        m_softRenderer.Write(object.Geometry());
        return;
      case mine::hardness_type::tough:
        m_toughRenderer.Write(object.Geometry());
        return;
    }
  }

private:

  geometry_renderer m_softRenderer { screen_render_brush_red.CreateBrush(), 6 };
  geometry_renderer m_toughRenderer { screen_render_brush_blue.CreateBrush(), 6 };

};
