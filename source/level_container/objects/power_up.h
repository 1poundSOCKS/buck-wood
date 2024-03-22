#pragma once

class power_up
{

public:

  power_up(POINT_2F position, VELOCITY_2F velocity) : m_position { position }, m_velocity { velocity }
  {
  }

  constexpr [[nodiscard]] auto Scale() const -> SCALE_2F { return m_scale; };
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
    m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
  }

  auto Destroy() -> void
  {
    m_destroyed = true;
  }

private:

  POINT_2F m_position;
  VELOCITY_2F m_velocity;
  constexpr static SCALE_2F m_scale { 20.0f, 20.0f };
  constexpr static float m_angle { 0 };
  bool m_destroyed { false };

};
