#pragma once

#include "framework.h"

class object_velocity
{

public:

  object_velocity();

  auto Update(VELOCITY_2F value, float interval) -> void;
  auto RelativeUpdate(float airResistance, float interval) -> void;

  [[nodiscard]] auto UpdatePosition(POINT_2F value, float interval) const noexcept -> POINT_2F;

private:

  VELOCITY_2F m_value;

};
