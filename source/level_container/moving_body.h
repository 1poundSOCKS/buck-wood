#pragma once

#include "game_point.h"
#include "game_velocity.h"

class moving_body
{

public:

  moving_body(game_point position, game_velocity velocity);

  auto SetPosition(const game_point& position) -> void;
  auto SetVelocity(const game_velocity& velocity) -> void;
  auto SetVelocity(float speed, float direction) -> void;
  auto Accelerate(const game_velocity& amount) -> void;
  auto Accelerate(float amount) -> void;
  auto Accelerate(float amount, float direction) -> void;
  auto SetDirection(float direction) -> void;

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Velocity() const -> const game_velocity&;

  auto Update(float interval) -> void;

  [[nodiscard]] auto Transform() const -> D2D1::Matrix3x2F;

protected:

  game_point m_position;// { 0, 0 };
  game_velocity m_velocity;// { 0, 0 };

};

// inline moving_body::moving_body(const game_point& position) : m_position { position }
// {
// }

inline moving_body::moving_body(game_point position, game_velocity velocity) : m_position { position }, m_velocity { velocity }
{
}

inline auto moving_body::SetPosition(const game_point& position) -> void
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

[[nodiscard]] inline auto moving_body::Position() const -> const game_point&
{
  return m_position;
}

[[nodiscard]] inline auto moving_body::Velocity() const -> const game_velocity&
{
  return m_velocity;
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
