#pragma once

#include "direct2d_functions.h"

class moving_body
{

public:

  moving_body(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity);

  auto SetPosition(D2D1_POINT_2F position) -> void;
  auto SetVelocity(direct2d::VELOCITY_2F velocity) -> void;
  auto SetVelocity(float speed, float direction) -> void;
  auto Accelerate(direct2d::VELOCITY_2F amount) -> void;
  auto Accelerate(float amount) -> void;
  auto Accelerate(float amount, float direction) -> void;
  auto SetDirection(float direction) -> void;

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Velocity() const -> direct2d::VELOCITY_2F;
  [[nodiscard]] auto Direction() const -> float;

  auto Update(float interval) -> void;

  [[nodiscard]] auto Transform() const -> D2D1::Matrix3x2F;

protected:

  D2D1_POINT_2F m_position;
  direct2d::VELOCITY_2F m_velocity;

};

inline moving_body::moving_body(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity) : m_position { position }, m_velocity { velocity }
{
}

inline auto moving_body::SetPosition(D2D1_POINT_2F position) -> void
{
  m_position = position;
}

inline auto moving_body::SetVelocity(direct2d::VELOCITY_2F velocity) -> void
{
  m_velocity = velocity;
}

inline auto moving_body::SetVelocity(float speed, float direction) -> void
{
  m_velocity = direct2d::CalculateVelocity(speed, direction);
}

inline auto moving_body::Accelerate(direct2d::VELOCITY_2F amount) -> void
{
  m_velocity = direct2d::CombineVelocities(m_velocity, amount);
}

inline auto moving_body::Accelerate(float amount) -> void
{
  m_velocity = direct2d::MultiplyVelocity(m_velocity, amount);
}

inline auto moving_body::SetDirection(float direction) -> void
{
  auto speed = direct2d::CalculateSpeed(m_velocity);
  m_velocity = direct2d::CalculateVelocity(speed, direction);
}

inline auto moving_body::Accelerate(float amount, float direction) -> void
{
  m_velocity = direct2d::CombineVelocities(m_velocity, direct2d::CalculateVelocity(amount, direction));
}

inline [[nodiscard]] auto moving_body::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto moving_body::Velocity() const -> direct2d::VELOCITY_2F
{
  return m_velocity;
}

inline [[nodiscard]] auto moving_body::Direction() const -> float
{
  return direct2d::CalculateDirection(m_velocity);
}

inline auto moving_body::Update(float interval) -> void
{
  m_position.x += m_velocity.x * interval;
  m_position.y += m_velocity.y * interval;
}

inline [[nodiscard]] auto moving_body::Transform() const -> D2D1::Matrix3x2F
{
  return D2D1::Matrix3x2F::Translation(m_position.x, m_position.y);
}
