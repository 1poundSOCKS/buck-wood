#include "pch.h"
#include "active_body.h"
#include "framework.h"

active_body::active_body(float thrust) : m_thrust(thrust)
{
}

auto active_body::SetPosition(float x, float y) -> void
{
  m_x = x;
  m_y = y;
}

auto active_body::SetAngle(float angle) -> void
{
  m_angle = angle;
}

auto active_body::SetThrust(bool enabled) -> void
{
  m_thrustEnabled = enabled;
}

[[nodiscard]] auto active_body::Position() const -> game_point
{
  return { m_x, m_y };
}

auto active_body::Update(int64_t ticks) -> void
{
  auto updateInterval = framework::gameUpdateInterval(ticks);

  m_velocityX -= ( ( m_velocityX * 0.4f ) * updateInterval );
  m_velocityY -= ( ( m_velocityY * 0.4f ) * updateInterval );

  float forceX = m_thrustEnabled ? m_thrust * sin(DEGTORAD(m_angle)) : 0.0f;
  float forceY = m_thrustEnabled ? -m_thrust * cos(DEGTORAD(m_angle)) : 0.0f;

  m_velocityX += forceX * updateInterval;
  m_velocityY += forceY * updateInterval;

  m_x += m_velocityX * updateInterval;
  m_y += m_velocityY * updateInterval;
}
