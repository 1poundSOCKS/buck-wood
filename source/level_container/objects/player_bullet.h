#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_velocity.h"

class player_bullet : public base_object
{

public:

  // player_bullet(POINT_2F position, VELOCITY_2F velocity, int damage);
  player_bullet(POINT_2F position, SCALE_2F scale, float angle);

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = m_velocity.UpdatePosition(m_position, interval);
  }

private:
 
  int m_damage;
  object_velocity m_velocity;

};
