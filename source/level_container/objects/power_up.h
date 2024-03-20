#pragma once

class power_up
{

public:

  power_up(POINT_2F position, float angle) : m_position { position }, m_angle { angle }
  {
  }

  constexpr [[nodiscard]] auto Scale() const -> SCALE_2F { return { 20.0f, 20.0f }; };
  constexpr [[nodiscard]] auto Angle() const -> float { return m_angle; }

  [[nodiscard]] auto Position() const -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return m_destroyed;
  }

  auto Update(float interval) -> void
  {
    constexpr float rotationSpeed = 100.0f;
    m_angle = direct2d::RotateAngle(m_angle, rotationSpeed * interval);
  }

  auto Destroy() -> void
  {
    m_destroyed = true;
  }

private:

  POINT_2F m_position;
  float m_angle;
  bool m_destroyed { false };

};
