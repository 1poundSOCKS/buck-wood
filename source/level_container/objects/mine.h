#pragma once

#include "framework.h"

class mine
{

public:

  enum type { one, two };

  mine(type type, POINT_2F position, VELOCITY_2F velocity);

  [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Type() const -> type;
  [[nodiscard]] auto PreviousPosition() const -> POINT_2F;

private:

  static constexpr float m_spinRate { 400 };
  
private:

  type m_type;
  POINT_2F m_position;
  POINT_2F m_previousPosition;
  VELOCITY_2F m_velocity { 0, 0 };
  float m_angle { 0 };
  bool m_destroyed { false };

};

inline [[nodiscard]] auto mine::Position() const -> POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto mine::PreviousPosition() const -> POINT_2F
{
  return m_previousPosition;
}

inline [[nodiscard]] auto mine::Angle() const -> float
{
  return m_angle;
}

inline [[nodiscard]] auto mine::Type() const -> type
{
  return m_type;
}
