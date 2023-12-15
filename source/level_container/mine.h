#pragma once

#include "framework.h"
#include "directional_body.h"
#include "dynamic_geometry.h"
#include "shape_generator.h"

class mine
{

public:

  mine() = default;
  mine(float x, float y);

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto PreviousPosition() const -> const game_point&;
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;
  auto Update(float interval, float x, float y) -> void;
  auto Update(float interval) -> void;

private:

  inline static const float m_spinRate { 500.0f };
  inline static const float m_thrustPower { 100.0f };
  directional_body m_body;
  directional_body m_previousState;
  game_angle m_spin;
  bool m_destroyed { false };
  dynamic_geometry m_geometry { shape_generator { 0, 0, 40, 40, 3 } };
  
};

[[nodiscard]] inline auto mine::Position() const -> const game_point&
{
  return m_body.Position();
}

[[nodiscard]] inline auto mine::PreviousPosition() const -> const game_point&
{
  return m_previousState.Position();
}

[[nodiscard]] inline auto mine::Geometry() const -> const transformed_path_geometry&
{
  return m_geometry;
}
