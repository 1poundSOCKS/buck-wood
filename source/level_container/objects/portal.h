#pragma once

#include "framework.h"

class portal
{

public:

  portal(POINT_2F position) : m_position { position }
  {
  }

  constexpr [[nodiscard]] auto Scale() const -> SCALE_2F
  {
    return m_scale;
  }

  constexpr [[nodiscard]] auto Angle() const -> float
  {
    return m_angle;
  }

  [[nodiscard]] auto Position() const -> POINT_2F
  {
    return m_position;
  }

  constexpr [[nodiscard]] auto Destroyed() const -> bool
  {
    return m_destroyed;
  }

  auto Update(float interval) -> void
  {
    m_age += interval;
  }

  auto Destroy() -> void
  {
  }

  [[nodiscard]] auto Age() const noexcept -> float
  {
    return m_age;
  }

private:

  POINT_2F m_position;
  constexpr static SCALE_2F m_scale { 150.0f, 150.0f };
  constexpr static float m_angle { 0 };
  constexpr static bool m_destroyed { false };
  constexpr static VELOCITY_2F m_velocity { 0, 0 };
  float m_age { 0 };

};
