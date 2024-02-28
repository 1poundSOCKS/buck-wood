#pragma once

#include "framework.h"

class player_bullet
{

public:

  player_bullet(POINT_2F position) : m_position { position }
  {
  }

  [[nodiscard]] auto Scale() const -> SCALE_2F
  {
    return m_scale;
  }

  [[nodiscard]] auto Angle() const -> float
  {
    return m_angle;
  }

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
  }

  auto Destroy() -> void
  {
    m_destroyed = true;
  }

private:

  POINT_2F m_position;
  SCALE_2F m_scale { 1.0f, 1.0f };
  float m_angle { 0 };
  bool m_destroyed { false };

};
