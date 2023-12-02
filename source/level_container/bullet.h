#pragma once

#include "play_event.h"
#include "moving_body.h"

class bullet
{
public:

  bullet(const game_point& position, const game_velocity& velocity, float angle);

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto DistanceTravelled() const -> float;
  [[nodiscard]] auto Range() const -> float;

  auto Update(float interval) -> void;
  auto Destroy() -> void;
  
private:

  inline static const float m_bulletSpeed = 500.0f;
  inline static const float m_bulletRange = 2000.0f;

  game_point m_startPosition = { 0, 0 };
  moving_body m_movingBody;
  float m_range = 1000;
  float m_distanceTravelled { 0 };
  bool m_destroyed = false;
  
};
