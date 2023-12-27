#include "pch.h"
#include "mine.h"

mine::mine(float x, float y) : m_body { game_point { x, y } }
{
}

auto mine::Update(float interval, const std::optional<game_point>& playerPosition) -> void
{
  m_previousState = m_body;

  if( playerPosition )
  {
    m_body.FacePosition(*playerPosition);
  }

  auto velocity = m_body.Velocity();
  float velocityChange = 1.0f - 0.1f * interval;
  velocity.Update(velocityChange, velocityChange);

  m_body.SetVelocity(velocity);
  m_body.Accelerate(m_thrustPower * interval);
  m_body.Update(interval);

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
