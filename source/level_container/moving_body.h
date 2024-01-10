#pragma once

#include "game_velocity.h"

class moving_body
{

public:

  moving_body(D2D1_POINT_2F position, game_velocity velocity);

  auto SetPosition(D2D1_POINT_2F position) -> void;
  auto SetVelocity(const game_velocity& velocity) -> void;
  auto SetVelocity(float speed, float direction) -> void;
  auto Accelerate(const game_velocity& amount) -> void;
  auto Accelerate(float amount) -> void;
  auto Accelerate(float amount, float direction) -> void;
  auto SetDirection(float direction) -> void;

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Velocity() const -> game_velocity;
  [[nodiscard]] auto Direction() const -> float;

  auto Update(float interval) -> void;

  [[nodiscard]] auto Transform() const -> D2D1::Matrix3x2F;

protected:

  D2D1_POINT_2F m_position;
  game_velocity m_velocity;

};

inline moving_body::moving_body(D2D1_POINT_2F position, game_velocity velocity) : m_position { position }, m_velocity { velocity }
{
}

inline auto moving_body::SetPosition(D2D1_POINT_2F position) -> void
{
  m_position = position;
}

inline auto moving_body::SetVelocity(const game_velocity& velocity) -> void
{
  m_velocity = velocity;
}

inline auto moving_body::SetVelocity(float speed, float direction) -> void
{
  m_velocity = game_velocity { game_angle { direction }, speed };
}

inline auto moving_body::Accelerate(const game_velocity& amount) -> void
{
  m_velocity += amount;
}

inline auto moving_body::Accelerate(float amount) -> void
{
  m_velocity.Accelerate(m_velocity.x() * amount, m_velocity.y() * amount);
}

inline auto moving_body::SetDirection(float direction) -> void
{
  auto speed = m_velocity.Speed();
  SetVelocity(speed, direction);
}

inline auto moving_body::Accelerate(float amount, float direction) -> void
{
  m_velocity.Accelerate(amount * sin(DEGTORAD(direction)), amount * cos(DEGTORAD(direction)));
}

inline [[nodiscard]] auto moving_body::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto moving_body::Velocity() const -> game_velocity
{
  return m_velocity;
}

inline [[nodiscard]] auto moving_body::Direction() const -> float
{
  return m_velocity.Direction();
}

inline auto moving_body::Update(float interval) -> void
{
  m_position.x += m_velocity.x() * interval;
  m_position.y += m_velocity.y() * interval;
}

inline [[nodiscard]] auto moving_body::Transform() const -> D2D1::Matrix3x2F
{
  return D2D1::Matrix3x2F::Translation(m_position.x, m_position.y);
}
