#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(const game_point& position, const game_velocity& velocity, float angle) : 
  m_startPosition { position }, m_movingBody { position, velocity }
{
  m_movingBody.Accelerate(game_velocity { angle, m_bulletSpeed });
}

[[nodiscard]] auto bullet::Position() const -> const game_point&
{
  return m_movingBody.Position();
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

auto bullet::Update(float interval) -> void
{
  m_movingBody.Update(interval);
  m_distanceTravelled = m_startPosition.DistanceTo(m_movingBody.Position());
  m_destroyed = m_distanceTravelled > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}