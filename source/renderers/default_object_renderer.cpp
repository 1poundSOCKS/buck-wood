#include "pch.h"
#include "default_object_renderer.h"
#include "geometry/level_geometries.h"
#include "geometric_object_transform.h"

default_object_renderer::default_object_renderer()
{
  auto baseGeometry = level_geometries::RectangleGeometry();
  auto transform = D2D1::Matrix3x2F::Scale({30,30}) * D2D1::Matrix3x2F::Translation({0, -70});
  m_enemy3_turretGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), baseGeometry.get(), transform);
}

auto default_object_renderer::Write(const default_object& object, ID2D1Geometry* geometry) const -> void
{
  object.Visit([this, geometry](auto&& object) { Write(object, geometry); });
}

auto default_object_renderer::Write(const background_object& object, ID2D1Geometry* geometry) const -> void
{
  m_backgroundObjectRenderer.Write(geometry);
}

auto default_object_renderer::Write(const enemy_ship& object, ID2D1Geometry* geometry) const -> void
{
  if( object.RemainingDamageInterval() > 0.0f )
  {
    m_damagedEnemyGeometryRenderer.Write(geometry);
  }
  else switch( object.Type() )
  {
    case enemy_ship::type::stalker:
      m_enemyType1_renderer.Write(object, geometry);
      break;

    case enemy_ship::type::random:
      m_enemyType2_renderer.Write(object, geometry);
      break;

    case enemy_ship::type::turret:
      m_enemyType3_renderer.Write(object, geometry);
      break;

    case enemy_ship::type::guard:
      m_enemyGuard_renderer.Write(object, geometry);
      break;
  }
}

auto default_object_renderer::Write(const player_ship& object, ID2D1Geometry* geometry) const -> void
{
  auto playerThrust = level_geometries::getPlayerThrust();
  auto scalePlayerThrust = D2D1::Matrix3x2F::Scale(1.0f, 0.2f + object.State().BaseThrusterPower() * 1.5f);
  auto movePlayerThrustDown = D2D1::Matrix3x2F::Translation(0.0f, 70.0f);
  auto shiftedThrustGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), playerThrust.get(), scalePlayerThrust * movePlayerThrustDown);
  auto transform = geometric_object_transform { object };
  auto transformedThrustGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), shiftedThrustGeometry.get(), transform.Get());
  m_playerThrustRenderer.Write(transformedThrustGeometry.get());

  m_playerShipRenderer.Write(object, geometry);
}

auto default_object_renderer::Write(const enemy_bullet& object, ID2D1Geometry* geometry) const -> void
{
  m_mineRenderer.Write(object, geometry);
}

auto default_object_renderer::Write(const power_up& object, ID2D1Geometry* geometry) const -> void
{
  m_powerUpRenderer.Write(geometry);
}

auto default_object_renderer::Write(const boundary_walls &object, ID2D1Geometry *geometry) const -> void
{
  m_insideLevelWallsRenderer.Write(geometry);
}
