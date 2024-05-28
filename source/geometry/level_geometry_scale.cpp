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

auto level_geometry_scale::player() -> SCALE_2F
{
  return m_instance->m_player;
}

auto level_geometry_scale::playerBullet() -> SCALE_2F
{
  return m_instance->m_playerBullet;
}

level_geometry_scale::level_geometry_scale() : 
  // m_player { Scale(level_geometries::player().get(), { 120, 120 }) },
  // m_playerBullet { Scale(level_geometries::playerBullet().get(), { 60, 60 }) }
  m_player { Scale(level_geometries::get(object_type::player).get(), { 120, 120 }) },
  m_playerBullet { Scale(level_geometries::get(object_type::player_bullet).get(), { 60, 60 }) }
{
}

auto level_geometry_scale::Scale(ID2D1Geometry *geometry, SIZE_F objectSize) -> SCALE_2F
{
  auto bounds = direct2d::GetGeometryBounds(geometry);
  auto geometryWidth = bounds.right - bounds.left;
  auto geometryHeight = bounds.bottom - bounds.top;
  return SCALE_2F( objectSize.width / geometryWidth, objectSize.height / geometryHeight );
}
