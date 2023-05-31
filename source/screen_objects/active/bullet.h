#ifndef _bullet_
#define _bullet_

#include "play_event.h"
#include "object_input_data.h"

class bullet
{
public:

  bullet(float x, float y, float angle);
  [[nodiscard]] auto Position() const -> game_point;
  auto Update(int64_t tickCount) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto HasExpired() -> bool;
  auto Destroy() -> void;

private:

  game_point m_startPosition = { 0, 0 };
  game_point m_position { 0, 0 };
  float m_xVelocity = 0;
  float m_yVelocity = 0;
  float m_angle = 0;
  float m_range = 1000;
  bool m_destroyed = false;
};

#endif
