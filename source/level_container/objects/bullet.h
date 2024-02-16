#pragma once

#include "direct2d_functions.h"

class bullet
{

public:

  enum class damage_mode { one, two };

public:

  bullet(D2D1_POINT_2F position, VELOCITY_2F velocity, damage_mode damageMode);

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto DistanceTravelled() const -> float;
  [[nodiscard]] auto Range() const -> float;
  [[nodiscard]] auto DamageMode() const -> damage_mode;

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
  bool m_destroyed = false;
  damage_mode m_damageMode;
  
};
