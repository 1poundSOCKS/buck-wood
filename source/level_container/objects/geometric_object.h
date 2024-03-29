#pragma once

#include "framework.h"

class geometric_object
{

public:

  geometric_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : m_position { position }, m_scale { scale }, m_angle { angle }
  {
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F
  {
    return m_scale;
  };

  [[nodiscard]] auto Angle() const noexcept -> float
  {
    return m_angle;
  }

  auto SetPosition(POINT_2F value) -> void
  {
    m_position = value;
  }

  auto SetScale(SCALE_2F value) -> void
  {
    m_scale = value;
  }

protected:

  POINT_2F m_position;
  SCALE_2F m_scale;
  float m_angle;

};
