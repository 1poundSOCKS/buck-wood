#pragma once

#include "geometry.h"

class moving_body
{
  public:

    moving_body() = default;
    moving_body(const game_point& position, const game_velocity& velocity);

    auto SetPosition(const game_point& position) -> void;
    auto SetVelocity(const game_velocity& velocity) -> void;
    auto Accelerate(const game_velocity& velocity) -> void;

    [[nodiscard]] auto Position() const -> const game_point&;
    [[nodiscard]] auto Velocity() const -> const game_velocity&;

    auto Update(float interval) -> void;

  private:

    game_point m_position { 0, 0 };
    game_velocity m_velocity { 0, 0 };

};

inline moving_body::moving_body(const game_point& position, const game_velocity& velocity) : m_position { position }, m_velocity { velocity }
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

inline auto moving_body::Accelerate(const game_velocity& velocity) -> void
{
  m_velocity += velocity;
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
  m_position.x += m_velocity.x * interval;
  m_position.y += m_velocity.y * interval;
}
