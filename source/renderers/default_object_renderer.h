#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "player_ship_renderer.h"
#include "level_target_renderer.h"
#include "mine_renderer.h"

class default_object_renderer
{

public:

  auto Write(const default_object& object, ID2D1Geometry* geometry) const -> void;

  auto Write(const level_target& object, ID2D1Geometry* geometry) const -> void
  {
    object.IsActivated() ? m_activatedRenderer.Write(geometry) : m_deactivatedRenderer.Write(geometry);
  }

  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
  {
    m_playerShipRenderer.Write(object, geometry);
  }

  auto Write(const mine& object, ID2D1Geometry* geometry) const -> void
  {
    m_mineRenderer.Write(object, geometry);
  }

  auto Write(const portal& object, ID2D1Geometry* geometry) const -> void
  {
    m_portalRenderer.Write(geometry);
  }

  auto Write(const auto& object, ID2D1Geometry* geometry) const -> void
  {
    m_defaultGeometryRenderer.Write(geometry);
  }

  geometry_renderer m_deactivatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_red.CreateBrush(), 10 };
  geometry_renderer m_activatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_grey.CreateBrush(), 10 };
  player_ship_renderer m_playerShipRenderer;
  mine_renderer m_mineRenderer;
  geometry_renderer m_portalRenderer { screen_render_brush_grey.CreateBrush(), 10 };
  geometry_renderer m_defaultGeometryRenderer { screen_render_brush_white.CreateBrush() };

};

struct default_object_renderer_visitor
{
  const default_object_renderer& m_renderer;
  ID2D1Geometry* m_geometry;

  auto operator()(const auto& object)
  {
    m_renderer.Write(object, m_geometry);
  }
};

inline auto default_object_renderer::Write(const default_object& object, ID2D1Geometry* geometry) const -> void
{
  std::visit(default_object_renderer_visitor { *this, geometry }, object.Get());
}
