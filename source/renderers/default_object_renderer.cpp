#include "pch.h"
#include "default_object_renderer.h"

struct default_object_renderer_visitor
{
  const default_object_renderer& m_renderer;
  ID2D1Geometry* m_geometry;

  auto operator()(const auto& object)
  {
    m_renderer.Write(object, m_geometry);
  }
};

default_object_renderer::default_object_renderer()
{
  auto baseGeometry = level_geometries::RectangleGeometry();
  auto transform = D2D1::Matrix3x2F::Scale({30,30}) * D2D1::Matrix3x2F::Translation({0, -70});
  m_enemy3_turretGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), baseGeometry.get(), transform);
}

auto default_object_renderer::Write(const default_object& object, ID2D1Geometry* geometry) const -> void
{
  object.Visit(default_object_renderer_visitor { *this, geometry });
}

auto default_object_renderer::Write(const level_cell &object, ID2D1Geometry *geometry) const -> void
{
  if( object.Type() == level_cell_type::wall )
  {
    m_floorCellRenderer.Write(geometry);
  }
}

auto default_object_renderer::Write(const background_object& object, ID2D1Geometry* geometry) const -> void
{
  m_backgroundObjectRenderer.Write(geometry);
}

auto default_object_renderer::Write(const enemy_type_1& object, ID2D1Geometry* geometry) const -> void
{
  switch( object.Type() )
  {
    case enemy_type_1::type::stalker:
      m_enemyType1_renderer.Write(object, geometry);
      break;
    case enemy_type_1::type::random:
      m_enemyType2_renderer.Write(object, geometry);
      break;
    case enemy_type_1::type::turret:
      m_enemyType3_renderer.Write(object, geometry);
      break;
  }
}

auto default_object_renderer::Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
{
  m_playerShipRenderer.Write(object, geometry);
}

auto default_object_renderer::Write(const enemy_bullet_1& object, ID2D1Geometry* geometry) const -> void
{
  m_mineRenderer.Write(object, geometry);
}

auto default_object_renderer::Write(const portal& object, ID2D1Geometry* geometry) const -> void
{
#ifdef _ENABLE_PORTAL_RENDERING
  m_portalRenderer.Write(object, geometry);
#endif
}

auto default_object_renderer::Write(const power_up& object, ID2D1Geometry* geometry) const -> void
{
  m_powerUpRenderer.Write(geometry);
}

auto default_object_renderer::Write(const boundary_walls &object, ID2D1Geometry *geometry) const -> void
{
  m_insideLevelWallsRenderer.Write(geometry);
}
