#pragma once

class play_events
{

public:

  enum class event_type { shot, explosion, target_activated };

  auto SetEvent(event_type eventType, bool value) -> void
  {
    switch( eventType )
    {
      case event_type::shot:
        m_shot = value;
        break;
      case event_type::explosion:
        m_explosion = value;
        break;
      case event_type::target_activated:
        m_targetActivated = value;
        break;
    }
  }

  [[nodiscard]] auto Get(event_type eventType) const -> bool
  {
    switch( eventType )
    {
      case event_type::shot:
        return m_shot;
      case event_type::explosion:
        return m_explosion;
      case event_type::target_activated:
        return m_targetActivated;
      default:
        return false;
    }
  }

  [[nodiscard]] auto operator[](event_type eventType) const -> bool
  {
    return Get(eventType);
  }

  auto Reset() -> void
  {
    m_shot = m_explosion = m_targetActivated = false;
  }

private:

  bool m_shot { false };
  bool m_explosion { false };
  bool m_targetActivated { false };

};
