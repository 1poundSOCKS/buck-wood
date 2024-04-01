#include "pch.h"
#include "play_events.h"

auto play_events::Set(event_type eventType, bool value) -> void
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
    case event_type::power_up_collected:
      m_powerUpCollected = value;
      break;
  }
}

[[nodiscard]] auto play_events::Get(event_type eventType) const -> bool
{
  switch( eventType )
  {
    case event_type::shot:
      return m_shot;
    case event_type::explosion:
      return m_explosion;
    case event_type::target_activated:
      return m_targetActivated;
    case event_type::power_up_collected:
      return m_powerUpCollected;
    default:
      return false;
  }
}

[[nodiscard]] auto play_events::operator[](event_type eventType) const -> bool
{
  return Get(eventType);
}

auto play_events::Reset() -> void
{
  m_shot = m_explosion = m_targetActivated = m_powerUpCollected = false;
}
