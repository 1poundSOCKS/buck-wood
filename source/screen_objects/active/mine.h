#pragma once

#include "transformed_path_geometry.h"
#include "directional_body.h"

class mine
{

public:

  mine() = default;
  mine(float x, float y);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;
  auto Update(int64_t tickCount, float x, float y) -> void;
  auto Update(int64_t tickCount) -> void;

private:

  inline static const float m_spinRate { 500.0f };
  inline static const float m_thrustPower { 100.0f };

  auto UpdateGeometry() -> void;
  [[nodiscard]] auto Transform() const -> D2D1::Matrix3x2F;

  directional_body m_body;
  game_angle m_spin;
  bool m_destroyed { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};

[[nodiscard]] inline auto mine::Transform() const -> D2D1::Matrix3x2F
{
  return D2D1::Matrix3x2F::Rotation(m_spin, { 0, 0 }) * m_body.Transform();
}
