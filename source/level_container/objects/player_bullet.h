#pragma once

#include "framework.h"

class player_bullet
{

public:

  player_bullet(POINT_2F position, VELOCITY_2F velocity) : m_position { position }, m_velocity { velocity }, m_angle { direct2d::CalculateDirection(velocity) }
  {
  }

  [[nodiscard]] auto Scale() const -> SCALE_2F
  {
    return m_scale;
  }

  [[nodiscard]] auto Angle() const -> float
  {
    return m_angle;
  }

  [[nodiscard]] auto Position() const -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return m_destroyed;
  }

  auto Update(float interval) -> void
  {
    m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
  }

  auto Destroy() -> void
  {
    m_destroyed = true;
  }

private:

  POINT_2F m_position;
  SCALE_2F m_scale { 1.0f, 1.0f };
  float m_angle;
  bool m_destroyed { false };
  VELOCITY_2F m_velocity;

};
