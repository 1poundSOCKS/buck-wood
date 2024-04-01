#pragma once

#include "framework.h"
#include "base_object.h"

class power_up : public base_object, public object_velocity
{

public:

  power_up(POINT_2F position, VELOCITY_2F velocity) : base_object { position , { 30.0f, 30.0f }, 0 }, object_velocity { velocity }
  {
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = object_velocity::UpdatePosition(m_position, interval);
  }

};
