#pragma once

#include "moving_body.h"

class directional_body : public moving_body
{
public:

  directional_body() = default;
  directional_body(const game_point& position);

  auto RotateClockwise(float angle) -> void;
  auto RotateAnticlockwise(float angle) -> void;

private:

  float m_angle { 0 };

};

inline directional_body::directional_body(const game_point& position) : moving_body { position }
{
}

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
