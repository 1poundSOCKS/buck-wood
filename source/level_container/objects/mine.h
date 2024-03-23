#pragma once

#include "framework.h"
#include "base_object.h"

class mine : public base_object
{

public:

  enum type { one, two };

  mine(type type, POINT_2F position, VELOCITY_2F velocity);

  [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;

  auto Update(float interval) -> void;

  [[nodiscard]] auto Type() const -> type;

private:

  static constexpr float m_spinRate { 400 };
  
private:

  constexpr static float rotationSpeed = 200.0f;

  type m_type;
  float m_angle { 0 };

};

inline [[nodiscard]] auto mine::Angle() const -> float
{
  return m_angle;
}

inline [[nodiscard]] auto mine::Type() const -> type
{
  return m_type;
}
