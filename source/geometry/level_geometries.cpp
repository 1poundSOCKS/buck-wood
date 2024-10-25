#include "pch.h"
#include "level_geometries.h"
#include "level_data.h"

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
  LoadAndCentreGeometryData(m_playerThrustPixelImage, { 16, 16 }, std::back_inserter(m_playerThrustData));
  auto shiftedThrustData = std::ranges::views::transform(m_playerThrustData, [](auto&& pointData) -> POINT_2F { return { pointData.x, pointData.y + 100.f }; });

  m_rectangleGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetRectangleGeometryData(), D2D1_FIGURE_END_CLOSED);
  m_mineGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 50, 50, 3 }, D2D1_FIGURE_END_CLOSED);
  m_targetGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 8 }, D2D1_FIGURE_END_CLOSED);
  m_player = LoadAndCentreGeometry(m_playerPixelImage, { 16, 16 });
  m_playerThrust = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shiftedThrustData, D2D1_FIGURE_END_CLOSED);
  m_playerBullet = LoadAndCentreGeometry(m_playerBulletPixelImage, { 40, 40 });
  m_enemy1 = LoadAndCentreGeometry(m_enemyStalkerPixelImage, { 16, 16 });
  m_enemy2 = LoadAndCentreGeometry(m_enemyStalkerPixelImage, { 16, 16 });
  m_enemy3 = LoadAndCentreGeometry(m_enemyTurretPixelImage, { 16, 16 });
  m_enemyBullet = LoadAndCentreGeometry(m_enemyBulletPixelImage, { 70, 70 });
  m_portal = LoadAndCentreGeometry(m_portalPixelImage, { 40, 40 });
  m_powerUp = LoadAndCentreGeometry(m_powerupPixelImage, { 20, 20 });

  int levelIndex = 0;
  std::vector<POINT_2F> boundaryData;

  while( level_data::LoadBoundaryData(levelIndex, std::back_inserter(boundaryData)) )
  {
    m_boundaryWalls.push_back(direct2d::CreatePathGeometry(d2d_factory::get_raw(), boundaryData, D2D1_FIGURE_END_CLOSED));
    ++levelIndex;
    boundaryData.clear();
  }

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

auto level_geometries::Get(const enemy_ship &object) -> winrt::com_ptr<ID2D1Geometry>
{
  switch( object.Type() )
  {
    case enemy_ship::type::stalker:
      return m_enemy1;
    case enemy_ship::type::random:
      return m_enemy2;
    case enemy_ship::type::turret:
      return m_enemy3;
    default:
      return m_rectangleGeometry;
  }
}

auto level_geometries::Get(const enemy_bullet &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_enemyBullet;
}

auto level_geometries::Get(const power_up &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_powerUp;
}

auto level_geometries::Get(const boundary_walls &object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_boundaryWalls[object.Level()];
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
