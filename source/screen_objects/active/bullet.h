#ifndef _bullet_
#define _bullet_

#include "play_event.h"
#include "object_input_data.h"
#include "geometry.h"

class bullet
{
public:

  bullet(float x, float y, float angle, game_velocity velocity);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto DistanceTravelled() const -> float;
  [[nodiscard]] auto Range() const -> float;

  auto Update(int64_t tickCount) -> void;
  auto Destroy() -> void;
  
private:

  game_point m_startPosition = { 0, 0 };
  game_point m_position { 0, 0 };
  game_velocity m_velocity { 0, 0 };
  float m_angle = 0;
  float m_range = 1000;
  float m_distanceTravelled { 0 };
  bool m_destroyed = false;
  
};

#endif
