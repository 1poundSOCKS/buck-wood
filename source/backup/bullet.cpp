#include "pch.h"
#include "bullet.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

bullet::bullet(D2D1_POINT_2F position, VELOCITY_2F velocity, damage_mode damageMode) : 
  m_startPosition { position }, m_position { position }, m_velocity { velocity }, m_damageMode { damageMode }
{
}

[[nodiscard]] auto bullet::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

[[nodiscard]] auto bullet::Destroyed() const -> bool
{
  return m_destroyed;
}

[[nodiscard]] auto bullet::DistanceTravelled() const -> float
{
  return direct2d::GetDistanceBetweenPoints(m_startPosition, m_position);
}

[[nodiscard]] auto bullet::Range() const -> float
{
  return m_range;
}

[[nodiscard]] auto bullet::DamageMode() const -> damage_mode
{
  return m_damageMode;
}

auto bullet::Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void
{
  auto direction = targetPosition ? std::optional<float>(direct2d::GetAngleBetweenPoints(m_position, *targetPosition)) : std::nullopt;
  m_velocity = direction ? direct2d::CalculateVelocity(m_bulletSpeed, *direction) : m_velocity;
  m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
  m_destroyed = DistanceTravelled() > m_range;
}

auto bullet::Destroy() -> void
{
  m_destroyed = true;
}
