#pragma once

#include "default_object.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "player_ship_renderer.h"
#include "flashing_geometry_renderer.h"
#include "mine_renderer.h"
#include "portal_renderer.h"

class default_object_renderer
{

public:

  default_object_renderer();

  auto Write(const default_object& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const background_object& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_ship& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_bullet& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const portal& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const power_up& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const boundary_walls& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const auto& object, ID2D1Geometry* geometry) const -> void;

private:

  geometry_renderer m_insideLevelWallsRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_grey.CreateBrush(), 20.0f };
  geometry_renderer m_backgroundObjectRenderer { screen_render_brush_very_dark_grey.CreateBrush() };
  color_scale_brushes m_targetBrushes { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 10 } };
  player_ship_renderer m_playerShipRenderer;
  geometry_renderer m_playerThrustRenderer { screen_render_brush_red.CreateBrush() };
  flashing_geometry_renderer m_playerMissileRenderer { color_scale { screen_render_brush_yellow.Get(), screen_render_brush_black.Get(), 20 } };
  flashing_geometry_renderer m_enemyType1_renderer { color_scale { screen_render_brush_blue.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  flashing_geometry_renderer m_enemyType2_renderer { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  flashing_geometry_renderer m_enemyType3_renderer { color_scale { screen_render_brush_cyan.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  flashing_geometry_renderer m_enemyGuard_renderer { color_scale { screen_render_brush_white.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  mine_renderer m_mineRenderer;
  portal_renderer m_portalRenderer;
  geometry_renderer m_powerUpRenderer { screen_render_brush_cyan.CreateBrush() };
  geometry_renderer m_defaultGeometryRenderer { screen_render_brush_white.CreateBrush() };
  geometry_renderer m_damagedEnemyGeometryRenderer { screen_render_brush_white.CreateBrush() };

  winrt::com_ptr<ID2D1Geometry> m_enemy3_turretGeometry;
};

auto default_object_renderer::Write(const auto& object, ID2D1Geometry* geometry) const -> void
{
  m_defaultGeometryRenderer.Write(geometry);
}
