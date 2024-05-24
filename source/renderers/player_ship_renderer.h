#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class player_ship_renderer
{

public:

  auto Write(const dynamic_object<player_ship>& object) const -> void
  {
    auto geometry = GetGeometry(object);
    m_rendererOne.Write(geometry.get());
  }

  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
  {
    m_rendererOne.Write(geometry);
  }

private:

  static [[nodiscard]] auto GetGeometry(const dynamic_object<player_ship>& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return object.GeometryPtr();
  }

  geometry_renderer m_rendererOne { screen_render_brush_type_one.CreateBrush() };

};
