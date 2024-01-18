#pragma once

#include "moving_body.h"

class homing_object : public moving_body
{

public:

  homing_object(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity, float thrust) : moving_body { position, velocity }, m_thrust { thrust }
  {
  }

  auto Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void
  {
    auto angleToTarget= targetPosition ? direct2d::GetAngleBetween(m_position, *targetPosition) : 0;
    float speedChange = m_thrust * interval;
    auto velocityChange = targetPosition ? direct2d::CalculateVelocity(speedChange, angleToTarget) : direct2d::VELOCITY_2F { 0, 0 };
    m_velocity = direct2d::CombineVelocities(m_velocity, velocityChange);
    moving_body::Update(interval);
  }

private:

  float m_thrust;

};
