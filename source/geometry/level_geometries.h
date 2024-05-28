#pragma once

#include "default_object.h"
#include "level_geometry_functions.h"

class level_geometries
{

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

  static [[nodiscard]] auto get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return std::visit([](const auto& object) { return get(object); }, defaultObject.Get());
  }

  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1PathGeometry>
  {
    auto objectType = level_objects::Type(object);
    return get(objectType);
  }

  static [[nodiscard]] auto get(object_type objectType) -> winrt::com_ptr<ID2D1PathGeometry>
  {
    switch( objectType )
    {
      case object_type::player:
        return m_instance->m_targetGeometry;
      case object_type::player_bullet:
        return m_instance->m_targetGeometry;
      case object_type::enemy_1:
        return m_instance->m_targetGeometry;
      case object_type::enemy_2:
        return m_instance->m_targetGeometry;
      case object_type::enemy_3:
        return m_instance->m_rectangleGeometry;
      case object_type::enemy_bullet_1:
        return m_instance->m_mineGeometry;
      case object_type::portal:
        return m_instance->m_targetGeometry;
      case object_type::power_up:
        return m_instance->m_targetGeometry;
      case object_type::wall:
        return m_instance->m_rectangleGeometry;
      default:
        return m_instance->m_rectangleGeometry;
    }
  }

  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_instance->m_rectangleGeometry;
  }

  static [[nodiscard]] auto CircleGeometry() -> winrt::com_ptr<ID2D1EllipseGeometry>
  {
    return m_instance->m_circleGeometry;
  }

  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&
  {
    return m_instance->m_hudTargetGeometries;
  }

private:

  level_geometries() : 
    m_rectangleGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetRectangleGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerShipGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerBulletGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerBulletGeometryData(), D2D1_FIGURE_END_CLOSED) },
    m_playerShieldGeometry { direct2d::CreateEllipseGeometry(d2d_factory::get_raw(), level_geometry_functions::GetPlayerShieldElipse()) },
    m_circleGeometry { direct2d::CreateEllipseGeometry(d2d_factory::get_raw(), level_geometry_functions::GetCircle()) }
  {
    LoadHudTargetGeometries(std::back_inserter(m_hudTargetGeometries));
  }

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

  inline static level_geometries* m_instance { nullptr };

  winrt::com_ptr<ID2D1PathGeometry> m_rectangleGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_playerShipGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_playerBulletGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_mineGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 50, 50, 3 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_targetGeometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 8 }, D2D1_FIGURE_END_CLOSED) };
  winrt::com_ptr<ID2D1PathGeometry> m_asteroidGeometry { level_geometry_functions::CreateAsteroidGeometry(0, 0, 200, 200) };
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;
  winrt::com_ptr<ID2D1EllipseGeometry> m_playerShieldGeometry;
  winrt::com_ptr<ID2D1EllipseGeometry> m_circleGeometry;

};
