#pragma once

#include "bullet.h"

class homing_bullet : public bullet
{

public:

  homing_bullet(game_point position, game_velocity velocity, std::optional<mine> target) : bullet(position, velocity), m_target { target }
  {
  }

  auto Update(float interval) -> void
  {
    bullet::Update(interval, m_target && !m_target->Destroyed() ? std::optional<game_point>(m_target->Position()) : std::nullopt);
  }

private:

  std::optional<mine> m_target;

};
