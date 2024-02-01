#pragma once

#include "framework.h"
#include "game_scale.h"

class mine
{

public:

  mine(D2D1_POINT_2F position);

  [[nodiscard]] auto Scale() const -> game_scale { return game_scale { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval, std::optional<D2D1_POINT_2F> playerPosition) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto PreviousPosition() const -> D2D1_POINT_2F;

private:

  static constexpr float m_spinRate { 400 };
  static constexpr float m_thrustPower { 10 };
  static constexpr float m_maxSpeed { 100 };

private:

  D2D1_POINT_2F m_position;
  D2D1_POINT_2F m_previousPosition;
  VELOCITY_2F m_velocity;
  float m_direction { 0 };
  float m_spin { 0 };
  bool m_destroyed { false };

};

inline [[nodiscard]] auto mine::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto mine::PreviousPosition() const -> D2D1_POINT_2F
{
  return m_previousPosition;
}

inline [[nodiscard]] auto mine::Angle() const -> float
{
  return m_spin;
}
