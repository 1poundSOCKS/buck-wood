#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(D2D1_POINT_2F position, VELOCITY_2F velocity) : 
  m_startPosition { position }, m_body { position, velocity }
{
}

[[nodiscard]] auto bullet::Position() const -> D2D1_POINT_2F
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

auto bullet::Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void
{
  m_body.SetDirection(targetPosition ? direct2d::GetAngleBetweenPoints(m_body.Position(), *targetPosition) : m_body.Direction());
  m_body.Update(interval);
  m_distanceTravelled = direct2d::GetDistanceBetweenPoints(m_startPosition, m_body.Position());
  m_destroyed = m_distanceTravelled > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
