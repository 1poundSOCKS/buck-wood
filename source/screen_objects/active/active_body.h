#pragma once

#include "geometry.h"

class active_body
{

public:

  enum class rotation_direction { none, clockwise, anticlockwise };

  active_body(float thrust);

  auto SetPosition(float x, float y) -> void;
  auto SetDirection(float angle) -> void;
  auto SetThrust(bool enabled) -> void;
  auto Rotate(rotation_direction rotationDirection, float angle) -> void;
  auto Update(float updateInterval) -> void;

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Direction() const -> float;
  [[nodiscard]] auto Transform() const -> D2D1::Matrix3x2F;

private:

  float m_x { 0 };
  float m_y { 0 };
  float m_velocityX { 0 };
  float m_velocityY { 0 };
  float m_direction { 0 };
  float m_angle { 0 };
  float m_thrust { 0 };
  bool m_thrustEnabled { false };

};
