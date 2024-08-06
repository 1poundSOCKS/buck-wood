#pragma once

#include "framework.h"
#include "base_object.h"

class player_bullet : public base_object
{

public:

  player_bullet(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);

    m_position.x += m_velocity.x * interval;
    m_position.y += m_velocity.y * interval;

    m_destroyed = m_age > 1.0f ? true : false;
  }

private:
 
  int m_damage;
  VELOCITY_2F m_velocity;

};
