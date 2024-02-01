#include "pch.h"
#include "mine.h"

mine::mine(D2D1_POINT_2F position) : m_position { position }, m_velocity { 0, 0 }, m_previousPosition { position }
{
}

auto mine::Update(float interval, std::optional<D2D1_POINT_2F> playerPosition) -> void
{
  m_previousPosition = m_position;

  m_direction = playerPosition ? direct2d::GetAngleBetweenPoints(m_position, *playerPosition) : m_direction;
  auto speed = std::min(direct2d::CalculateSpeed(m_velocity) + m_thrustPower, m_maxSpeed);
  m_velocity = direct2d::CalculateVelocity(speed, m_direction);
  m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);

  m_spin += m_spinRate * interval;
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
