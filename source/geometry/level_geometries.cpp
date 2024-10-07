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
  m_player = LoadPixelGeometry(m_playerPixelImage, { 16, 16 });
  m_playerBullet = LoadPixelGeometry(m_playerBulletPixelImage, { 40, 40 });
  m_enemy1 = LoadPixelGeometry(m_enemyStalkerPixelImage, { 16, 16 });
  m_enemy2 = LoadPixelGeometry(m_enemyStalkerPixelImage, { 16, 16 });
  m_enemy3 = LoadPixelGeometry(m_enemyTurretPixelImage, { 16, 16 });
  m_enemyBullet1 = LoadPixelGeometry(m_enemyBulletPixelImage, { 50, 50 });
  m_portal = LoadPixelGeometry(m_portalPixelImage, { 40, 40 });
  m_powerUp = LoadPixelGeometry(m_powerupPixelImage, { 20, 20 });
  m_boundaryWalls = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetBoundaryWallsGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_boundaryLeftExit = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetBoundaryLeftExitGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_boundaryTopExit = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetBoundaryTopExitGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_boundaryRightExit = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetBoundaryRightExitGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_boundaryBottomExit = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetBoundaryBottomExitGeometryData(), D2D1_FIGURE_END_CLOSED);

  LoadHudTargetGeometries(std::back_inserter(m_hudTargetGeometries));
}

auto level_geometries::Get(const player_ship &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_player;
}

auto level_geometries::Get(const player_bullet &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_playerBullet;
}

auto level_geometries::Get(const enemy_type_1 &object) -> winrt::com_ptr<ID2D1Geometry>
{
  switch( object.Type() )
  {
    case enemy_type_1::type::stalker:
      return m_enemy1;
    case enemy_type_1::type::random:
      return m_enemy2;
    case enemy_type_1::type::turret:
      return m_enemy3;
    default:
      return m_rectangleGeometry;
  }
}

auto level_geometries::Get(const enemy_bullet_1 &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_enemyBullet1;
}

auto level_geometries::Get(const boundary_exit &object) -> winrt::com_ptr<ID2D1Geometry>
{
  switch( object.Type() )
  {
    case boundary_exit::type::left:
      return m_boundaryLeftExit;
    case boundary_exit::type::top:
      return m_boundaryTopExit;
    case boundary_exit::type::right:
      return m_boundaryRightExit;
    case boundary_exit::type::bottom:
      return m_boundaryBottomExit;
    default:
      return m_boundaryLeftExit;
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

auto level_geometries::Scale(ID2D1Geometry *geometry, SIZE_F size) -> SCALE_2F
{
  auto bounds = direct2d::GetGeometryBounds(geometry);
  auto geometryWidth = bounds.right - bounds.left;
  auto geometryHeight = bounds.bottom - bounds.top;
  return SCALE_2F( size.width / geometryWidth, size.height / geometryHeight );
}
