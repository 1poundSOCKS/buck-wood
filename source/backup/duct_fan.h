#pragma once

#include "framework.h"

class duct_fan
{

public:

  duct_fan(D2D1_POINT_2F position, float rotationSpeed) : 
    m_angle { static_cast<float>(m_angleDist(pseudo_random_generator::get())) }, m_position { position }, m_rotationSpeed { rotationSpeed }
  {
  }

  [[nodiscard]] auto Scale() const -> SCALE_2F { return m_scale; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  constexpr [[nodiscard]] auto Destroyed() const -> bool { return false; }

  auto Update(float interval) -> void
  {
    m_angle = direct2d::RotateAngle(m_angle, m_rotationSpeed * interval);
  }

private:

  inline static std::uniform_int_distribution<int> m_angleDist { 0, 359 };
  SCALE_2F m_scale { 0.5f, 1.0f };
  float m_angle { 0 };
  D2D1_POINT_2F m_position;
  float m_rotationSpeed { 0 };

};

inline [[nodiscard]] auto duct_fan::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto duct_fan::Angle() const -> float
{
  return m_angle;
}
