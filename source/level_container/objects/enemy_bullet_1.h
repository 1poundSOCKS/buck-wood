#pragma once

#include "framework.h"
#include "base_object.h"

class enemy_bullet_1 : public base_object
{

public:

  enemy_bullet_1(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  [[nodiscard]] auto Expired() const -> bool
  {
    return m_age > m_lifespan;
  }

  auto Update(float interval) -> void;

private:

  static constexpr float m_lifespan { 15.0f };
  static constexpr float m_rotationSpeed { 300.0f };

  VELOCITY_2F m_velocity;

};
