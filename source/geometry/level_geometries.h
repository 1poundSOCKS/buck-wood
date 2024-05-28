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

  static [[nodiscard]] auto get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>
  {
    return std::visit([](const auto& object) { return get(object); }, defaultObject.Get());
  }

  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    auto objectType = level_objects::Type(object);
    return get(objectType);
  }

  static [[nodiscard]] auto get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_instance->Get(objectType);
  }

  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_instance->m_rectangleGeometry;
  }

  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&
  {
    return m_instance->m_hudTargetGeometries;
  }


private:

  level_geometries();

  [[nodiscard]] auto Get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
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
      case object_type::wall:
        return m_rectangleGeometry;
      default:
        return m_rectangleGeometry;
    }
  }

  [[nodiscard]] auto ScaledGeometry(object_type objectType, SIZE_F geometrySize) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F size) -> SCALE_2F;

  [[nodiscard]] auto GetBase(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
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
        return m_rectangleGeometry;
      case object_type::enemy_bullet_1:
        return m_mineGeometry;
      case object_type::portal:
        return m_targetGeometry;
      case object_type::power_up:
        return m_targetGeometry;
      case object_type::wall:
        return m_rectangleGeometry;
      default:
        return m_rectangleGeometry;
    }
  }

  static [[nodiscard]] auto LoadHudTargetGeometries(auto&& geometryInserter) -> void
  {
    auto topLeftGeometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), level_geometry_functions::GetHudTargetTopLeftGeometryData(), D2D1_FIGURE_END_OPEN);
    geometryInserter = topLeftGeometry;
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(90));
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(180));
    geometryInserter = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), topLeftGeometry.get(), D2D1::Matrix3x2F::Rotation(270));
  }

private:

  inline static level_geometries* m_instance { nullptr };

  winrt::com_ptr<ID2D1Geometry> m_rectangleGeometry;
  winrt::com_ptr<ID2D1Geometry> m_mineGeometry;
  winrt::com_ptr<ID2D1Geometry> m_targetGeometry;
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;

  winrt::com_ptr<ID2D1Geometry> m_player;
  winrt::com_ptr<ID2D1Geometry> m_playerBullet;
  winrt::com_ptr<ID2D1Geometry> m_enemy1;
  winrt::com_ptr<ID2D1Geometry> m_enemy2;
  winrt::com_ptr<ID2D1Geometry> m_enemy3;
  winrt::com_ptr<ID2D1Geometry> m_enemyBullet1;
  winrt::com_ptr<ID2D1Geometry> m_portal;
  winrt::com_ptr<ID2D1Geometry> m_powerUp;

};
