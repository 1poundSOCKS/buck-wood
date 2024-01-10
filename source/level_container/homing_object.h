#pragma once

#include "moving_body.h"

class homing_object : public moving_body
{

public:

  homing_object(D2D1_POINT_2F position, game_velocity velocity, float thrust) : moving_body { position, velocity }, m_thrust { thrust }
  {
  }

  auto Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void
  {
    auto angleToTarget= targetPosition ? direct2d::GetAngleBetween(m_position, *targetPosition) : 0;
    float speedChange = m_thrust * interval;
    game_velocity velocityChange = targetPosition ? game_velocity { angleToTarget, speedChange } : game_velocity { 0, 0 };
    m_velocity += velocityChange;
    moving_body::Update(interval);
  }

private:

  float m_thrust;

};
