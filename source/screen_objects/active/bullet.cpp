#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(float x, float y, float angle, game_velocity velocity) : 
  m_startPosition { x, y }, m_position { x, y }, m_angle(angle), m_velocity { velocity }
{
  static const float bulletSpeed = 500.0f;
  static const float bulletRange = 2000.0f;
  
  m_velocity.x += bulletSpeed * sin(DEGTORAD(m_angle));
  m_velocity.y += -bulletSpeed * cos(DEGTORAD(m_angle));
}

[[nodiscard]] auto bullet::Position() const -> game_point
{
  return m_position;
}

[[nodiscard]] auto bullet::Destroyed() const -> bool
{
  return m_destroyed;
}

[[nodiscard]] auto bullet::DistanceTravelled() const -> float
{
  return m_distanceTravelled;
}

[[nodiscard]] auto bullet::Range() const -> float
{
  return m_range;
}

auto bullet::Update(int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);
  m_position.x += ( m_velocity.x * updateInterval );
  m_position.y += ( m_velocity.y * updateInterval );
  m_distanceTravelled = m_startPosition.DistanceTo(m_position);
  m_destroyed = m_distanceTravelled > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
