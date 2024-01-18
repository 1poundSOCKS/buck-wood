#pragma once

#include "moving_body.h"
#include "game_angle.h"

class directional_body : public moving_body
{
public:

  directional_body(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity, float angle);

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto RotateClockwise(float angle) -> void;
  auto RotateAnticlockwise(float angle) -> void;
  auto FacePosition(D2D1_POINT_2F position) -> void;

  auto Accelerate(float value) -> void;

  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto RelativePosition(float angle, float cx, float cy) const -> D2D1_POINT_2F;

private:

  float m_angle { 0 };

};

inline directional_body::directional_body(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity, float angle) : moving_body { position, velocity }, m_angle { angle }
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

inline auto directional_body::FacePosition(D2D1_POINT_2F position) -> void
{
  m_angle = direct2d::GetAngleBetween(m_position, position);
}

inline auto directional_body::Accelerate(float amount) -> void
{
  moving_body::Accelerate(amount, m_angle);
}

[[nodiscard]] inline auto directional_body::Angle() const -> float
{
  return m_angle;
}

inline [[nodiscard]] auto directional_body::RelativePosition(float angle, float cx, float cy) const -> D2D1_POINT_2F
{
  game_angle relativeAngle { m_angle };
  relativeAngle += angle;
  auto transform = D2D1::Matrix3x2F::Rotation(relativeAngle);
  auto result = transform.TransformPoint({cx, cy});
  return { m_position.x + result.x, m_position.y + result.y };
}
