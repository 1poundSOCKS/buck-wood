#pragma once

struct event_player_dead
{
public:

  event_player_dead(float x, float y, std::function<void(float,float)> event) : m_x(x), m_y(y), m_event(event)
  {
  }

  auto Trigger() const -> void
  {
    m_event(m_x, m_y);
  }

private:

  float m_x = 0;
  float m_y = 0;
  std::function<void(float,float)> m_event;
};
