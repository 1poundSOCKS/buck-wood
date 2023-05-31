#include "pch.h"
#include "bullet.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(float x, float y, float angle) : m_startPosition { x, y }, m_position { x, y }, m_angle(angle)
{
  static const float bulletSpeed = 500.0f;
  static const float bulletRange = 2000.0f;
  
  m_xVelocity = bulletSpeed * sin(DEGTORAD(m_angle));
  m_yVelocity = -bulletSpeed * cos(DEGTORAD(m_angle));
}

[[nodiscard]] auto bullet::Position() const -> game_point
{
  return m_position;
}

auto bullet::Update(int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);
  
  m_position.x += ( m_xVelocity * updateInterval );
  m_position.y += ( m_yVelocity * updateInterval );

  m_destroyed = HasExpired();
}

[[nodiscard]] auto bullet::Destroyed() const -> bool
{
  return m_destroyed;
}

[[nodiscard]] auto bullet::HasExpired() -> bool
{
  float cx = m_position.x - m_startPosition.x;
  float cy = m_position.y - m_startPosition.y;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
