#pragma once

#include "framework.h"

class mine
{

public:

  enum hardness_type { soft, tough };

  mine(D2D1_POINT_2F position, float thrust, float maxSpeed, hardness_type hardnessType);

  [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval, std::optional<POINT_2F> playerPosition) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto HardnessType() const -> hardness_type;
  [[nodiscard]] auto PreviousPosition() const -> POINT_2F;

private:

  static constexpr float m_spinRate { 400 };
  
private:

  hardness_type m_hardnessType;
  POINT_2F m_position;
  float m_thrust;
  float m_maxSpeed;

  POINT_2F m_previousPosition;
  VELOCITY_2F m_velocity { 0, 0 };
  float m_direction { 0 };
  float m_spin { 0 };
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
  return m_spin;
}

inline [[nodiscard]] auto mine::HardnessType() const -> hardness_type
{
  return m_hardnessType;
}
