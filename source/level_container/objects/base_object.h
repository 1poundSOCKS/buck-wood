#pragma once

#include "framework.h"

class base_object
{

public:

  base_object(POINT_2F position, VELOCITY_2F velocity, SCALE_2F scale, float angle) noexcept : 
    m_position { position }, m_velocity { velocity }, m_scale { scale }, m_angle { angle }
  {
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F
  {
    return m_velocity;
  }

  [[nodiscard]] auto Age() const noexcept -> float
  {
    return m_age;
  }

  [[nodiscard]] auto Destroyed() const noexcept -> bool
  {
    return m_destroyed;
  }

  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F
  {
    return m_scale;
  };

  [[nodiscard]] auto Angle() const noexcept -> float
  {
    return m_angle;
  }

  auto Update(float interval) noexcept -> void
  {
    m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
    m_age += interval;
  }

  auto Destroy() noexcept -> void
  {
    m_destroyed = true;
  }

protected:

  POINT_2F m_position;
  VELOCITY_2F m_velocity;
  float m_age { 0 };
  bool m_destroyed { false };
  SCALE_2F m_scale;
  float m_angle;

};
