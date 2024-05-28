#include "pch.h"
#include "level_geometries.h"

level_geometries::level_geometries()
{
  m_rectangleGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetRectangleGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_mineGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 50, 50, 3 }, D2D1_FIGURE_END_CLOSED);
  m_targetGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 8 }, D2D1_FIGURE_END_CLOSED);
  m_player = ScaledGeometry(object_type::player, { 120, 120 });
  m_playerBullet = ScaledGeometry(object_type::player_bullet, { 60, 60 });
  m_enemy1 = ScaledGeometry(object_type::enemy_1, { 160, 160 });
  m_enemy2 = ScaledGeometry(object_type::enemy_2, { 140, 140 });
  m_enemy3 = ScaledGeometry(object_type::enemy_3, { 160, 160 });
  m_enemyBullet1 = ScaledGeometry(object_type::enemy_bullet_1, { 60, 60 });
  m_portal = ScaledGeometry(object_type::portal, { 200, 200 });
  m_powerUp = ScaledGeometry(object_type::power_up, { 60, 60 });
  LoadHudTargetGeometries(std::back_inserter(m_hudTargetGeometries));
}

auto level_geometries::ScaledGeometry(object_type objectType, SIZE_F geometrySize) -> winrt::com_ptr<ID2D1Geometry>
{
  auto baseGeometry = GetBase(objectType);
  auto scale = Scale(baseGeometry.get(), geometrySize);
  auto transform = D2D1::Matrix3x2F::Scale({scale.x, scale.y});
  return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), baseGeometry.get(), transform);
}

auto level_geometries::Scale(ID2D1Geometry *geometry, SIZE_F size) -> SCALE_2F
{
  auto bounds = direct2d::GetGeometryBounds(geometry);
  auto geometryWidth = bounds.right - bounds.left;
  auto geometryHeight = bounds.bottom - bounds.top;
  return SCALE_2F( size.width / geometryWidth, size.height / geometryHeight );
}
