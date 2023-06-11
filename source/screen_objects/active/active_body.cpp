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

auto active_body::SetDirection(float angle) -> void
{
  m_direction = angle;
}

auto active_body::SetThrust(bool enabled) -> void
{
  m_thrustEnabled = enabled;
}

auto active_body::Rotate(rotation_direction rotationDirection, float angle) -> void
{
  switch( rotationDirection )
  {
  case rotation_direction::clockwise:
    m_angle += angle;
    if( m_angle >= 360 ) m_angle -= 360;
    break;
  case rotation_direction::anticlockwise:
    m_angle -= angle;
    if( m_angle < 0 ) m_angle += 360;
    break;
  }
}

auto active_body::Update(float updateInterval) -> void
{
  m_velocityX -= ( ( m_velocityX * 0.4f ) * updateInterval );
  m_velocityY -= ( ( m_velocityY * 0.4f ) * updateInterval );

  float forceX = m_thrustEnabled ? m_thrust * sin(DEGTORAD(m_direction)) : 0.0f;
  float forceY = m_thrustEnabled ? -m_thrust * cos(DEGTORAD(m_direction)) : 0.0f;

  m_velocityX += forceX * updateInterval;
  m_velocityY += forceY * updateInterval;

  m_x += m_velocityX * updateInterval;
  m_y += m_velocityY * updateInterval;
}

[[nodiscard]] auto active_body::Position() const -> game_point
{
  return { m_x, m_y };
}

[[nodiscard]] auto active_body::Direction() const -> float
{
  return m_direction;
}

[[nodiscard]] auto active_body::Transform() const -> D2D1::Matrix3x2F
{
  return D2D1::Matrix3x2F::Rotation(m_angle, { 0, 0 }) * D2D1::Matrix3x2F::Translation(m_x, m_y);
}
