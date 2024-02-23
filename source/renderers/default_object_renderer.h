#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "player_ship_renderer.h"
#include "level_target_renderer.h"
#include "mine_renderer.h"

struct default_object_renderer_visitor
{
  const geometry_renderer& m_deactivatedRenderer;
  const geometry_renderer& m_activatedRenderer;
  const player_ship_renderer& m_playerShipRenderer;
  const mine_renderer& m_mineRenderer;
  ID2D1Geometry* m_geometry;

  auto operator()(const level_target& object)
  {
    object.IsActivated() ? m_activatedRenderer.Write(m_geometry) : m_deactivatedRenderer.Write(m_geometry);
  }

  auto operator()(const player_ship& object)
  {
    m_playerShipRenderer.Write(object, m_geometry);
  }

  auto operator()(const mine& object)
  {
    m_mineRenderer.Write(object, m_geometry);
  }

  auto operator()(const auto& object)
  {
  }
};

class default_object_renderer
{

public:

  auto Write(const default_object& object, ID2D1Geometry* geometry) const -> void;

private:

  geometry_renderer m_deactivatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_red.CreateBrush(), 10 };
  geometry_renderer m_activatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_grey.CreateBrush(), 10 };
  player_ship_renderer m_playerShipRenderer;
  mine_renderer m_mineRenderer;

};

inline auto default_object_renderer::Write(const default_object& object, ID2D1Geometry* geometry) const -> void
{
  default_object_renderer_visitor visitor { m_deactivatedRenderer, m_activatedRenderer, m_playerShipRenderer, m_mineRenderer, geometry };
  std::visit(visitor, object.Get());
}
