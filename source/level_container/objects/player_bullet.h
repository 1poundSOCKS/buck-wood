#pragma once

#include "framework.h"
#include "base_object.h"

class player_bullet : public base_object, public object_velocity
{

public:

  player_bullet(POINT_2F position, VELOCITY_2F velocity, int damage) : base_object { position, { 0.6f, 0.6f }, direct2d::CalculateDirection(velocity) }, object_velocity { velocity },
    m_damage { damage }
  {
  }

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = UpdatePosition(m_position, interval);
  }

private:

  int m_damage;

};
