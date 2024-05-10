#pragma once

#include "level_objects.h"
#include "dynamic_object.h"

class targetted_object
{

public:

  targetted_object(dynamic_object<default_object>* object) : m_object { object }
  {
  }

  [[nodiscard]] auto Object() const -> const default_object&
  {
    return m_object->Object();
  }

  [[nodiscard]] auto Object() -> default_object&
  {
    return m_object->Object();
  }

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F
  {
    return Object().Position();
  }

  [[nodiscard]] auto Bounds(D2D1::Matrix3x2F transform) const -> D2D1_RECT_F
  {
    auto unrotatedTransform = transform = m_object->CreateUnrotatedTransform() * transform;
    auto topLeft = unrotatedTransform.TransformPoint({-30, -30});
    auto bottomRight = unrotatedTransform.TransformPoint({30, 30});
    return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
  }

  auto Destroy() -> void
  {
    Object().Destroy();
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return Object().Destroyed();
  }

  [[nodiscard]] auto CanBeDestroyed(const player_ship& playerShip) -> bool
  {
    // auto targettedBullet = Object().GetIf<enemy_bullet_1>();
    // return targettedBullet && CanBeDestroyed(*targettedBullet, playerShip) ? true : false;
    return true;
  }

private:

  // static [[nodiscard]] auto CanBeDestroyed(const enemy_bullet_1& bullet, const player_ship& playerShip) -> bool
  // {
    // return bullet.Type() == enemy_bullet_1::type::one && playerShip.FireMode() == player_ship::fire_mode::two || 
    //     bullet.Type() == enemy_bullet_1::type::two && playerShip.FireMode() == player_ship::fire_mode::one;
  //   return true;
  // }

private:

  dynamic_object<default_object>* m_object;

};
