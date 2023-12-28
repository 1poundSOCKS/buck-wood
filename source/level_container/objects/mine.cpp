#include "pch.h"
#include "mine.h"

mine::mine(game_point position) : m_body { position, game_velocity { 0, 0 }, m_thrustPower }, m_previousState { m_body }
{
}

auto mine::Update(float interval, const std::optional<game_point>& playerPosition) -> void
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
