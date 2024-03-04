#include "pch.h"
#include "mine.h"

mine::mine(type type, POINT_2F position, VELOCITY_2F velocity) : 
  m_type { type }, m_position { position }, m_previousPosition { position }, m_velocity { velocity }
{
}

auto mine::Update(float interval) -> void
{
  m_previousPosition = m_position;
  m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
