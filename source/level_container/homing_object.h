#pragma once

#include "moving_body.h"

class homing_object : public moving_body
{

public:

  homing_object(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity, float thrust, float maxSpeed)
    : moving_body { position, velocity }, m_thrust { thrust }, m_maxSpeed { maxSpeed }
  {
  }

  auto Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void
  {
    float maxSpeed = m_maxSpeed * interval;
    float newSpeed = std::min(moving_body::Speed() + m_thrust * interval, maxSpeed);
    auto angleToTarget = targetPosition ? direct2d::GetAngleBetween(m_position, *targetPosition) : moving_body::Direction();
    moving_body::SetDirection(angleToTarget);
    moving_body::SetSpeed(newSpeed);
    moving_body::Update(interval);
  }

private:

  float m_maxSpeed;
  float m_thrust;

};
