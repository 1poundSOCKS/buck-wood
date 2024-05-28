#pragma once

#include "framework.h"
#include "level_objects.h"

class level_geometry_scale
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto get(object_type objectType) -> SCALE_2F
  {
    switch( objectType )
    {
      case object_type::player:
        return m_instance->m_player;
      case object_type::player_bullet:
        return m_instance->m_playerBullet;
      case object_type::enemy_1:
        return m_instance->m_enemy1;
      case object_type::enemy_2:
        return m_instance->m_enemy2;
      case object_type::enemy_3:
        return m_instance->m_enemy3;
      case object_type::enemy_bullet_1:
        return m_instance->m_enemyBullet1;
      case object_type::portal:
        return m_instance->m_portal;
      case object_type::power_up:
        return m_instance->m_powerUp;
      case object_type::wall:
        return m_instance->m_wall;
      default:
        return SCALE_2F { 1, 1 };
    }
  }

  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1PathGeometry>
  {
    auto objectType = level_objects::Type(object);
    return get(objectType);
  }

private:

  level_geometry_scale();

  static [[nodiscard]] auto Scale(object_type objectType, SIZE_F objectSize) -> SCALE_2F;
  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F objectSize) -> SCALE_2F;

private:

  inline static level_geometry_scale* m_instance { nullptr };

  SCALE_2F m_player;
  SCALE_2F m_playerBullet;
  SCALE_2F m_enemy1;
  SCALE_2F m_enemy2;
  SCALE_2F m_enemy3;
  SCALE_2F m_enemyBullet1;
  SCALE_2F m_portal;
  SCALE_2F m_powerUp;
  SCALE_2F m_wall;

};
