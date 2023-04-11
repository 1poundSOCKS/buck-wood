#pragma once

#include "bullet.h"

struct event_player_shot
{
  event_player_shot(bullet newBullet, std::function<void(bullet)> event) : newBullet(newBullet), event(event)
  {
  }

  auto Trigger() const -> void
  {
    event(newBullet);
  }

  bullet newBullet;
  std::function<void(bullet)> event;
};
