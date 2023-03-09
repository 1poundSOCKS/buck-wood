#pragma once

#include "player_ship.h"

struct event_player_shot
{
  event_player_shot(float x, float y, float angle, std::function<void(float,float,float)> event) : x(x), y(y), angle(angle), event(event)
  {
  }

  auto Trigger() const -> void
  {
    event(x, y, angle);
  }

  float x;
  float y;
  float angle;
  std::function<void(float,float,float)> event;
};
