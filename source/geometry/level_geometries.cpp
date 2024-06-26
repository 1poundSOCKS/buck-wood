#include "pch.h"
#include "level_geometries.h"

auto level_geometries::create() -> void
{
  destroy();
  m_instance = new level_geometries();
}

auto level_geometries::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

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
  m_enemyBullet1 = ScaledGeometry(object_type::enemy_bullet_1, { 80, 80 });
  m_portal = ScaledGeometry(object_type::portal, { 200, 200 });
  m_powerUp = ScaledGeometry(object_type::power_up, { 60, 60 });
  LoadHudTargetGeometries(std::back_inserter(m_hudTargetGeometries));
}

auto level_geometries::Get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
{
  switch( objectType )
  {
    case object_type::player:
      return m_player;
    case object_type::player_bullet:
      return m_playerBullet;
    case object_type::enemy_1:
      return m_enemy1;
    case object_type::enemy_2:
      return m_enemy2;
    case object_type::enemy_3:
      return m_enemy3;
    case object_type::enemy_bullet_1:
      return m_enemyBullet1;
    case object_type::portal:
      return m_portal;
    case object_type::power_up:
      return m_powerUp;
    case object_type::cell:
      return m_rectangleGeometry;
    default:
      return m_rectangleGeometry;
  }
}

auto level_geometries::GetBase(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
{
  switch( objectType )
  {
    case object_type::player:
      return m_targetGeometry;
    case object_type::player_bullet:
      return m_targetGeometry;
    case object_type::enemy_1:
      return m_targetGeometry;
    case object_type::enemy_2:
      return m_targetGeometry;
    case object_type::enemy_3:
      return m_targetGeometry;
    case object_type::enemy_bullet_1:
      return m_mineGeometry;
    case object_type::portal:
      return m_targetGeometry;
    case object_type::power_up:
      return m_targetGeometry;
    case object_type::cell:
      return m_rectangleGeometry;
    default:
      return m_rectangleGeometry;
  }
}

auto level_geometries::LoadHudTargetGeometries(auto&& geometryInserter) -> void
{
  auto topLeftGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetHudTargetTopLeftGeometryData(), D2D1_FIGURE_END_OPEN);
  geometryInserter = topLeftGeometry;
  geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(90));
  geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(180));
  geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(270));
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
