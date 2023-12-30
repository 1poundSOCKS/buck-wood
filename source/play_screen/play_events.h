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
        m_data->m_shot = value;
        break;
      case event_type::explosion:
        m_data->m_explosion = value;
        break;
      case event_type::target_activated:
        m_data->m_targetActivated = value;
        break;
    }
  }

  [[nodiscard]] auto operator[](event_type eventType) const -> bool
  {
    switch( eventType )
    {
      case event_type::shot:
        return m_data->m_shot;
      case event_type::explosion:
        return m_data->m_explosion;
      case event_type::target_activated:
        return m_data->m_targetActivated;
      default:
        return false;
    }
  }

  auto Reset() -> void
  {
    m_data->m_shot = m_data->m_explosion = m_data->m_targetActivated = false;
  }

private:

  struct data
  {
    bool m_shot { false };
    bool m_explosion { false };
    bool m_targetActivated { false };
  };

  std::shared_ptr<data> m_data { std::make_shared<data>() };

};
