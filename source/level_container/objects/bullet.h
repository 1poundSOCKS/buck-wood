#pragma once

#include "play_event.h"
#include "homing_object.h"

class bullet
{
public:

  bullet(game_point position, game_velocity velocity);

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto DistanceTravelled() const -> float;
  [[nodiscard]] auto Range() const -> float;

  auto Update(float interval, std::optional<game_point> targetPosition) -> void;
  auto Destroy() -> void;
  
private:

  inline static const float m_bulletSpeed = 500.0f;
  inline static const float m_thrust = 500.0f;
  inline static const float m_bulletRange = 2000.0f;

  game_point m_startPosition = { 0, 0 };
  homing_object m_homingObject;
  float m_range = 1000;
  float m_distanceTravelled { 0 };
  bool m_destroyed = false;
  
};
