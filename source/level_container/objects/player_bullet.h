#pragma once

#include "framework.h"
#include "base_object.h"

class player_bullet : public base_object
{

public:

  player_bullet(POINT_2F position, VELOCITY_2F velocity, int damage) : base_object { position, velocity, { 1.5f, 1.5f }, direct2d::CalculateDirection(velocity) },
    m_damage { damage }
  {
  }

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

private:

  int m_damage;

};
