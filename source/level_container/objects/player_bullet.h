#pragma once

#include "geometry/level_geometry_scale.h"

class player_bullet : public base_object
{

public:

  player_bullet(POINT_2F position, VELOCITY_2F velocity, int damage) : 
    base_object { position, level_geometry_scale::playerBullet(), direct2d::CalculateDirection(velocity) }, 
    m_velocity { velocity }, m_damage { damage }
  {
  }

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = m_velocity.UpdatePosition(m_position, interval);
    auto x = level_geometry_scale::playerBullet();

  }

private:
 
  int m_damage;
  object_velocity m_velocity;

};
