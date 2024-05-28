#include "pch.h"
#include "level_geometry_scale.h"
#include "level_geometries.h"
#include "direct2d_functions.h"

auto level_geometry_scale::create() -> void
{
  destroy();
  m_instance = new level_geometry_scale();
}

auto level_geometry_scale::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

level_geometry_scale::level_geometry_scale() : 
  m_player { Scale(object_type::player, { 120, 120 }) },
  m_playerBullet { Scale(object_type::player_bullet, { 60, 60 }) },
  m_enemy1 { Scale(object_type::enemy_1, { 160, 160 }) },
  m_enemy2 { Scale(object_type::enemy_2, { 140, 140 }) },
  m_enemy3 { Scale(object_type::enemy_3, { 160, 160 }) },
  m_enemyBullet1 { Scale(object_type::enemy_bullet_1, { 60, 60 }) },
  m_portal { Scale(object_type::portal, { 200, 200 }) },
  m_powerUp { Scale(object_type::power_up, { 60, 60 }) }
{
}

[[nodiscard]] auto level_geometry_scale::Scale(object_type objectType, SIZE_F objectSize) -> SCALE_2F
{
  return Scale(level_geometries::get(objectType).get(), objectSize);
}

auto level_geometry_scale::Scale(ID2D1Geometry *geometry, SIZE_F objectSize) -> SCALE_2F
{
  auto bounds = direct2d::GetGeometryBounds(geometry);
  auto geometryWidth = bounds.right - bounds.left;
  auto geometryHeight = bounds.bottom - bounds.top;
  return SCALE_2F( objectSize.width / geometryWidth, objectSize.height / geometryHeight );
}
