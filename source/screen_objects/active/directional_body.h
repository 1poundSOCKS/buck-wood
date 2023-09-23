#pragma once

#include "moving_body.h"

class directional_body : public moving_body
{
public:

  directional_body() = default;
  auto RotateClockwise(float angle) -> void;
  auto RotateAnticlockwise(float angle) -> void;

private:

  float m_angle { 0 };

};

inline auto directional_body::RotateClockwise(float angle) -> void
{
  m_angle += angle;
  if( m_angle >= 360 ) m_angle -= 360;
}

inline auto directional_body::RotateAnticlockwise(float angle) -> void
{
  m_angle -= angle;
  if( m_angle < 0 ) m_angle += 360;
}
