#include "pch.h"
#include "mine.h"
#include "shape_generator.h"

mine::mine(float x, float y) : m_body { game_point { x, y } }, m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_geometry.Load( shape_generator { 0, 0, 40, 40, 3 } );
  UpdateGeometry();
}

[[nodiscard]] auto mine::Position() const -> game_point
{
  return m_body.Position();
}

[[nodiscard]] auto mine::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
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
  auto updateInterval = framework::gameUpdateInterval(tickCount);
  m_spin += m_spinRate * updateInterval;
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
