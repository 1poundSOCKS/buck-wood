#include "pch.h"
#include "mine.h"

#define HOMING_MINE

mine::mine(POINT_2F position, float thrust, float maxSpeed, type type, POINT_2F targetPosition) : 
  m_position { position }, m_previousPosition { position }, m_thrust { thrust }, m_maxSpeed { maxSpeed }, m_type { type }
{
  m_direction = direct2d::GetAngleBetweenPoints(position, targetPosition);
  m_velocity = direct2d::CalculateVelocity(maxSpeed, m_direction);
}

auto mine::Update(float interval, std::optional<POINT_2F> targetPosition) -> void
{
  m_previousPosition = m_position;

#ifdef HOMING_MINE
  auto targetDirection = targetPosition ? direct2d::GetAngleBetweenPoints(m_position, *targetPosition) : m_direction;
  auto velocityChange = direct2d::CalculateVelocity(m_thrust * interval, targetDirection);
  m_velocity = direct2d::CombineVelocities(m_velocity, velocityChange);
#else
  m_direction = targetPosition ? direct2d::GetAngleBetweenPoints(m_position, *targetPosition) : m_direction;
  auto speed = std::min(direct2d::CalculateSpeed(m_velocity) + m_thrust, m_maxSpeed);
  m_velocity = direct2d::CalculateVelocity(speed, m_direction);
#endif

  m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
  m_spin = direct2d::RotateAngle(m_spin, m_spinRate * interval);
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
