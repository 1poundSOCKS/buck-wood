#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "level_geometries.h"

class player_ship_renderer
{

public:

  auto Write(const dynamic_object<player_ship>& object) const -> void
  {
    auto geometry = GetGeometry(object);

    switch( object->FireMode() )
    {
      case player_ship::fire_mode::two:
        m_rendererTwo.Write(geometry.get());
        return;

      default:
        m_rendererOne.Write(geometry.get());
        return;
    }
  }

  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
  {
    switch( object.FireMode() )
    {
      case player_ship::fire_mode::two:
        m_rendererTwo.Write(geometry);
        return;

      default:
        m_rendererOne.Write(geometry);
        return;
    }
  }

private:

  static [[nodiscard]] auto GetGeometry(const dynamic_object<player_ship>& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    auto transform = object.Transform();
    return object->ShieldsUp() ? 
      winrt::com_ptr<ID2D1Geometry> { direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), level_geometries::PlayerShipGeometry().get(), transform) } : 
      winrt::com_ptr<ID2D1Geometry> { object.GeometryPtr() };
  }

  geometry_renderer m_rendererOne { screen_render_brush_type_one.CreateBrush() };
  geometry_renderer m_rendererTwo { screen_render_brush_type_two.CreateBrush() };

};
