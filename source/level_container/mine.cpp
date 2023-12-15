#include "pch.h"
#include "mine.h"
#include "directional_body_transform.h"

mine::mine(float x, float y) : m_body { game_point { x, y } }, m_geometry { shape_generator { 0, 0, 40, 40, 3 }, directional_body_transform { m_body } }
{
}

auto mine::Update(float interval, float x, float y) -> void
{
  auto position = m_body.Position();
  auto angle = CalculateAngle(position.x, position.y, x, y);
  m_body.SetAngle(angle);
  Update(interval);
}

auto mine::Update(float interval) -> void
{
  m_previousState = m_body;

  m_spin += m_spinRate * interval;

  auto velocity = m_body.Velocity();
  float velocityChange = 1.0f - 0.1f * interval;
  velocity.Update(velocityChange, velocityChange);

  m_body.SetVelocity(velocity);
  m_body.Accelerate(m_thrustPower * interval);
  m_body.Update(interval);

  m_geometry.Transform(D2D1::Matrix3x2F::Rotation(m_spin) * directional_body_transform { m_body });
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
