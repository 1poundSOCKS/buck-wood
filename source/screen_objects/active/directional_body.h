#pragma once

#include "moving_body.h"

class directional_body : public moving_body
{
public:

  directional_body() = default;
  directional_body(const game_point& position);

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto RotateClockwise(float angle) -> void;
  auto RotateAnticlockwise(float angle) -> void;

  auto Accelerate(float value) -> void;

  [[nodiscard]] auto Angle() const -> float;

private:

  float m_angle { 0 };

};

inline directional_body::directional_body(const game_point& position) : moving_body { position }
{
}

inline auto directional_body::SetAngle(float angle) -> void
{
  m_angle = angle;  
}

inline auto directional_body::Rotate(float angle) -> void
{
  m_angle += angle;
  
  while( m_angle < 0 )
  {
    m_angle += 360;
  }
  
  while( m_angle >= 360 )
  {
    m_angle -= 360;
  }
}

inline auto directional_body::RotateClockwise(float angle) -> void
{
  m_angle += angle;

  while( m_angle >= 360 )
  {
    m_angle -= 360;
  }
}

inline auto directional_body::RotateAnticlockwise(float angle) -> void
{
  m_angle -= angle;

  while( m_angle < 0 )
  {
    m_angle += 360;
  }
}

inline auto directional_body::Accelerate(float amount) -> void
{
  moving_body::Accelerate( game_velocity { m_angle, amount } );
}

[[nodiscard]] inline auto directional_body::Angle() const -> float
{
  return m_angle;
}
