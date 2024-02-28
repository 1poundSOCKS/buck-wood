#pragma once

#include "framework.h"
#include "level_objects.h"
#include "level_geometry_functions.h"

class level_geometries
{

private:

  inline static level_geometries* m_instance { nullptr };

  level_geometries() : 
    m_playerShipGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerBulletGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerBulletGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerShieldGeometry { direct2d::CreateEllipseGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerShieldElipse()) }
  {
    LoadHudTargetGeometries(std::back_inserter(m_hudTargetGeometries));
  }

public:

  static auto create() -> void
  {
    destroy();
    m_instance = new level_geometries();
  }

  static auto destroy() -> void
  {
    if( m_instance )
    {
      delete m_instance;
      m_instance = nullptr;
    }
  }

  static [[nodiscard]] auto PlayerShipGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_playerShipGeometry.get();
  }

  static [[nodiscard]] auto PlayerBulletGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_playerBulletGeometry.get();
  }

  static [[nodiscard]] auto PlayerShieldGeometry() -> ID2D1EllipseGeometry*
  {
    return m_instance->m_playerShieldGeometry.get();
  }

  static [[nodiscard]] auto MineGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_mineGeometry.get();
  }

  static [[nodiscard]] auto TargetGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_targetGeometry.get();
  }

  static [[nodiscard]] auto AsteroidGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_asteroidGeometry.get();
  }

  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&
  {
    return m_instance->m_hudTargetGeometries;
  }

  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_instance->Get(object);
  }

private:

  static [[nodiscard]] auto LoadHudTargetGeometries(auto&& geometryInserter) -> void
  {
    auto topLeftGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetHudTargetTopLeftGeometryData(), D2D1_FIGURE_END_OPEN);
    geometryInserter = topLeftGeometry;
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(90));
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(180));
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(270));
  }

  [[nodiscard]] auto Get(const player_ship& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_playerShipGeometry;
  }

  [[nodiscard]] auto Get(const level_target& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_targetGeometry;
  }

  [[nodiscard]] auto Get(const mine& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_mineGeometry;
  }

private:

  winrt::com_ptr<ID2D1PathGeometry> m_playerShipGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_playerBulletGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_mineGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 40, 40, 3 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_targetGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 6 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_asteroidGeometry { level_geometry_functions::CreateAsteroidGeometry(0, 0, 200, 200) };
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;
  winrt::com_ptr<ID2D1EllipseGeometry> m_playerShieldGeometry;

};
