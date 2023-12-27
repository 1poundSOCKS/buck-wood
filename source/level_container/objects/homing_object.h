#pragma once

#include "directional_body.h"

class homing_object : public directional_body
{

public:

  homing_object(game_point position, float thrustPower) : directional_body(position), m_thrustPower { thrustPower }
  {
  }

  auto Update(float interval, std::optional<game_point> targetPosition) -> void
  {
    if( targetPosition )
    {
      FacePosition(*targetPosition);
    }

    float velocityChange = 1.0f - 0.1f * interval;
    m_velocity.Update(velocityChange, velocityChange);
    Accelerate(m_thrustPower * interval);
    moving_body::Update(interval);
  }

private:

  float m_thrustPower;

};
