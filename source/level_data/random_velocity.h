#pragma once

#include "framework.h"

class random_velocity
{

public:

  random_velocity(int minSpeed, int maxSpeed) : m_velocityDist { minSpeed, maxSpeed }
  {
  }

  [[nodiscard]] auto get() noexcept -> VELOCITY_2F
  {
    auto angle = static_cast<float>(m_angleDist(pseudo_random_generator::get()));
    auto speed = static_cast<float>(m_velocityDist(pseudo_random_generator::get()));
    return direct2d::CalculateVelocity(speed, angle);
  }

private:

  inline static std::uniform_int_distribution<int> m_angleDist { 0, 359 };
  std::uniform_int_distribution<int> m_velocityDist;

};
