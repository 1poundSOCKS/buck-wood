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

  auto Write(const mine& object, ID2D1Geometry* geometry) const -> void
  {
    switch( object.HardnessType() )
    {
      case mine::hardness_type::soft:
        m_softRenderer.Write(geometry);
        return;
      case mine::hardness_type::tough:
        m_toughRenderer.Write(geometry);
        return;
    }
  }

private:

  geometry_renderer m_softRenderer { screen_render_brush_type_one.CreateBrush(), 6 };
  geometry_renderer m_toughRenderer { screen_render_brush_type_two.CreateBrush(), 6 };

};
