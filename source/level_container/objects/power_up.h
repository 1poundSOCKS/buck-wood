#pragma once

#include "framework.h"
#include "base_object.h"

class power_up : public base_object
{

public:

  power_up(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position , scale, angle }, m_velocity { 0, 0 }
  {
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = m_velocity.UpdatePosition(m_position, interval);
  }

private:

  object_velocity m_velocity;

};
