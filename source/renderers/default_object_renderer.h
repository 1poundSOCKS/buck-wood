#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class default_object_renderer
{

public:

  auto Write(const default_object& object, ID2D1Geometry* geometry) const -> void;

private:

  geometry_renderer m_deactivatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_red.CreateBrush(), 10 };
  geometry_renderer m_activatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_grey.CreateBrush(), 10 };
  player_ship_renderer m_playerShipRenderer;

};

inline auto default_object_renderer::Write(const default_object& object, ID2D1Geometry* geometry) const -> void
{
  struct visitor
  {
    const geometry_renderer& m_deactivatedRenderer;
    const geometry_renderer& m_activatedRenderer;
    const player_ship_renderer& m_playerShipRenderer;
    ID2D1Geometry* m_geometry;

    auto operator()(const level_target& object)
    {
      object.IsActivated() ? m_activatedRenderer.Write(m_geometry) : m_deactivatedRenderer.Write(m_geometry);
    }
    auto operator()(const player_ship& object)
    {
      m_playerShipRenderer.Write(object, m_geometry);
    }
  };

  std::visit(visitor { m_deactivatedRenderer, m_activatedRenderer, m_playerShipRenderer, geometry }, object.Get());
}