#pragma once

struct event_player_shot
{
  event_player_shot(float x, float y, float angle, std::function<void(float,float,float)> event) : x(x), y(y), angle(angle), event(event)
  {
  }

  auto Trigger() const -> void
  {
    event(x, y, angle);
  }

  float x = 0;
  float y = 0;
  float angle = 0;
  std::function<void(float,float,float)> event;
};
