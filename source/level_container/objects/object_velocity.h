#pragma once

#include "framework.h"

class object_velocity
{

public:

  object_velocity() noexcept = default;
  
  object_velocity(VELOCITY_2F velocity) noexcept : m_velocity { velocity }
  {
  }

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F
  {
    return m_velocity;
  }

  auto UpdatePosition(POINT_2F position, float interval) noexcept -> POINT_2F
  {
    return direct2d::CalculatePosition(position, m_velocity, interval);
  }

protected:

  VELOCITY_2F m_velocity { 0, 0 };

};
