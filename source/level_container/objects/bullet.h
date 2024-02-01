#pragma once

#include "direct2d_functions.h"

class bullet
{
public:

  bullet(D2D1_POINT_2F position, VELOCITY_2F velocity);

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto DistanceTravelled() const -> float;
  [[nodiscard]] auto Range() const -> float;

  auto Update(float interval, std::optional<D2D1_POINT_2F> targetPosition) -> void;
  auto Destroy() -> void;
  
private:

  inline static const float m_bulletSpeed = 500.0f;
  inline static const float m_thrust = 500.0f;
  inline static const float m_bulletRange = 2000.0f;

  D2D1_POINT_2F m_startPosition;
  D2D1_POINT_2F m_position;
  VELOCITY_2F m_velocity;
  float m_direction;
  float m_range = 1000;
  float m_distanceTravelled { 0 };
  bool m_destroyed = false;
  
};
