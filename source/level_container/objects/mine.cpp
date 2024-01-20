#include "pch.h"
#include "mine.h"

mine::mine(D2D1_POINT_2F position) : m_body { position, { 0, 0 }, m_thrustPower, m_maxSpeed }, m_previousState { m_body }
{
}

auto mine::Update(float interval, std::optional<D2D1_POINT_2F> playerPosition) -> void
{
  m_previousState = m_body;
  m_body.Update(interval, playerPosition);
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
