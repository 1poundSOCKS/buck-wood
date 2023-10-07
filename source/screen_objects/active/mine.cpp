#include "pch.h"
#include "mine.h"
#include "shape_generator.h"

mine::mine(float x, float y) : m_body { game_point { x, y } }, m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_geometry.Load( shape_generator { 0, 0, 40, 40, 3 } );
  UpdateGeometry();
}

auto mine::Update(int64_t tickCount, float x, float y) -> void
{
  auto position = m_body.Position();
  auto angle = CalculateAngle(position.x, position.y, x, y);
  m_body.SetAngle(angle);
  Update(tickCount);
}

auto mine::Update(int64_t tickCount) -> void
{
  m_previousState = m_body;

  auto updateInterval = framework::gameUpdateInterval(tickCount);
  m_spin += m_spinRate * updateInterval;

  auto velocity = m_body.Velocity();
  float velocityChange = 1.0f - 0.1f * updateInterval;
  velocity.Update(velocityChange, velocityChange);
  m_body.SetVelocity(velocity);

  m_body.Accelerate(m_thrustPower * updateInterval);
  
  m_body.Update(updateInterval);
  UpdateGeometry();
}

auto mine::UpdateGeometry() -> void
{
  auto transform = Transform();
  m_transformedGeometry = { m_geometry, transform };
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
