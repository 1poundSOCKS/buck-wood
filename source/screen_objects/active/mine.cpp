#include "pch.h"
#include "mine.h"
#include "shape_generator.h"

mine::mine(float x, float y) : m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_body.SetPosition(x, y);
  m_body.SetThrust(true);
  game_rect rect = { { - 20, - 20 }, { 20, 20} };
  m_geometry.Load( shape_generator<clean_shape> { rect, 4 } );
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

auto mine::UpdateGeometry() -> void
{
  auto position = m_body.Position();
  auto moveTransform = D2D1::Matrix3x2F::Translation(position.x, position.y);
  m_transformedGeometry = { m_geometry, moveTransform };
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_destroyed;
}

auto mine::Destroy() -> void
{
  m_destroyed = true;
}
