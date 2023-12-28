#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(game_point position, game_velocity velocity) : 
  m_startPosition { position }, m_homingObject { position, velocity, m_thrust }
{
}

[[nodiscard]] auto bullet::Position() const -> const game_point&
{
  return m_homingObject.Position();
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

auto bullet::Update(float interval, std::optional<game_point> targetPosition) -> void
{
  m_homingObject.Update(interval, targetPosition);
  m_distanceTravelled = m_startPosition.DistanceTo(m_homingObject.Position());
  m_destroyed = m_distanceTravelled > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
