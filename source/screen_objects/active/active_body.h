#pragma once

#include "geometry.h"

class active_body
{

public:

  active_body(float thrust);

  auto SetPosition(float x, float y) -> void;
  auto SetAngle(float angle) -> void;
  auto SetThrust(bool enabled) -> void;
  [[nodiscard]] auto Position() const -> game_point;

  auto Update(int64_t ticks) -> void;

private:

  float m_x { 0 };
  float m_y { 0 };
  float m_velocityX { 0 };
  float m_velocityY { 0 };
  float m_angle { 0 };
  float m_thrust { 0 };
  bool m_thrustEnabled { false };

};
