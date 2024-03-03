#pragma once

#include "health_status.h"

struct player_state
{
  D2D1_POINT_2F m_position { 0, 0 };
  D2D1_POINT_2F m_previousPosition { 0, 0 };
  float m_angle { 0 };
  VELOCITY_2F m_velocity { 0, 0 };
  bool m_destroyed { false };
  bool m_thrusterOn { false };
  health_status m_shieldStatus { 10 };
  bool m_active { false };

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return m_destroyed;
  }

  [[nodiscard]] auto Position() const -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Shields() const -> const health_status&
  {
    return m_shieldStatus;
  }

  [[nodiscard]] auto ThrusterOn() const -> bool
  {
    return !m_destroyed && m_thrusterOn;
  }
};
