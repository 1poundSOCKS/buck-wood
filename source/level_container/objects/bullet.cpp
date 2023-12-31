#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(game_point position, game_velocity velocity) : 
  m_startPosition { position }, m_body { position, velocity }
{
}

[[nodiscard]] auto bullet::Position() const -> const game_point&
{
  return m_body.Position();
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
  m_body.Update(interval);
  m_distanceTravelled = m_startPosition.DistanceTo(m_body.Position());
  m_destroyed = m_distanceTravelled > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
