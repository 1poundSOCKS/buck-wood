#pragma once

#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "player_ship_renderer.h"
#include "flashing_geometry_renderer.h"
#include "mine_renderer.h"
#include "portal_renderer.h"

class default_object_renderer
{

public:

  auto Write(const default_object& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const background_object& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_type_1& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_type_2& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_type_3& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const player_ship& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const player_missile& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const enemy_bullet_1& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const portal& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const power_up& object, ID2D1Geometry* geometry) const -> void;
  auto Write(const auto& object, ID2D1Geometry* geometry) const -> void;

private:

  geometry_renderer m_backgroundObjectRenderer { screen_render_brush_very_dark_grey.CreateBrush() };
  color_scale_brushes m_targetBrushes { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 10 } };
  player_ship_renderer m_playerShipRenderer;
  flashing_geometry_renderer m_playerMissileRenderer { color_scale { screen_render_brush_yellow.Get(), screen_render_brush_black.Get(), 20 } };
  flashing_geometry_renderer m_enemyType1_renderer { color_scale { screen_render_brush_blue.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  flashing_geometry_renderer m_enemyType2_renderer { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  flashing_geometry_renderer m_enemyType3_renderer { color_scale { screen_render_brush_cyan.Get(), screen_render_brush_black.Get(), 20 }, 10 };
  mine_renderer m_mineRenderer;
  portal_renderer m_portalRenderer;
  geometry_renderer m_powerUpRenderer { screen_render_brush_cyan.CreateBrush() };
  geometry_renderer m_defaultGeometryRenderer { screen_render_brush_white.CreateBrush() };

  friend struct default_object_renderer_visitor;
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

inline auto default_object_renderer::Write(const background_object& object, ID2D1Geometry* geometry) const -> void
{
  m_backgroundObjectRenderer.Write(geometry);
}

inline auto default_object_renderer::Write(const enemy_type_1& object, ID2D1Geometry* geometry) const -> void
{
  m_enemyType1_renderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const enemy_type_2& object, ID2D1Geometry* geometry) const -> void
{
  m_enemyType2_renderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const enemy_type_3& object, ID2D1Geometry* geometry) const -> void
{
  m_enemyType3_renderer.Write(object, geometry);
  auto baseGeometry = level_geometries::RectangleGeometry();
  auto objectTransform = geometric_object_transform { object };
  auto transform = D2D1::Matrix3x2F::Scale({0.3,0.4}) * D2D1::Matrix3x2F::Translation({0, -0.4}) * objectTransform.Get();
  auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), baseGeometry.get(), transform);
  m_defaultGeometryRenderer.Write(transformedGeometry.get());
}

inline auto default_object_renderer::Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
{
  m_playerShipRenderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const player_missile& object, ID2D1Geometry* geometry) const -> void
{
  m_playerMissileRenderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const enemy_bullet_1& object, ID2D1Geometry* geometry) const -> void
{
  m_mineRenderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const portal& object, ID2D1Geometry* geometry) const -> void
{
  m_portalRenderer.Write(object, geometry);
}

inline auto default_object_renderer::Write(const power_up& object, ID2D1Geometry* geometry) const -> void
{
  m_powerUpRenderer.Write(geometry);
}

inline auto default_object_renderer::Write(const auto& object, ID2D1Geometry* geometry) const -> void
{
  m_defaultGeometryRenderer.Write(geometry);
}
