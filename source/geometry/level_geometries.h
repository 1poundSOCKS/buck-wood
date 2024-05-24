#pragma once

#include "framework.h"
#include "level_objects.h"
#include "level_geometry_functions.h"

class level_geometries
{

private:

  inline static level_geometries* m_instance { nullptr };

  level_geometries() : 
    m_rectangleGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetRectangleGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerShipGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerBulletGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerBulletGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerShieldGeometry { direct2d::CreateEllipseGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerShieldElipse()) },
    m_circleGeometry { direct2d::CreateEllipseGeometry(d2d_factory::get_raw(), level_geometry_functions::GetCircle()) }
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

  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_rectangleGeometry;
  }

  static [[nodiscard]] auto PlayerShipGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_playerShipGeometry;
  }

  static [[nodiscard]] auto PlayerBulletGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_playerBulletGeometry;
  }

  static [[nodiscard]] auto PlayerShieldGeometry() -> ID2D1EllipseGeometry*
  {
    return m_instance->m_playerShieldGeometry.get();
  }

  static [[nodiscard]] auto CircleGeometry() -> winrt::com_ptr<ID2D1EllipseGeometry>
  {
    return m_instance->m_circleGeometry;
  }

  static [[nodiscard]] auto MineGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_mineGeometry;
  }

  static [[nodiscard]] auto TargetGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_targetGeometry;
  }

  static [[nodiscard]] auto AsteroidGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_asteroidGeometry;
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

  [[nodiscard]] auto Get(const enemy_type_2& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_targetGeometry;
  }

  [[nodiscard]] auto Get(const enemy_bullet_1& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_mineGeometry;
  }

private:

  winrt::com_ptr<ID2D1PathGeometry> m_rectangleGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_playerShipGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_playerBulletGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_mineGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 50, 50, 3 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_targetGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 6 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_asteroidGeometry { level_geometry_functions::CreateAsteroidGeometry(0, 0, 200, 200) };
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;
  winrt::com_ptr<ID2D1EllipseGeometry> m_playerShieldGeometry;
  winrt::com_ptr<ID2D1EllipseGeometry> m_circleGeometry;

};
