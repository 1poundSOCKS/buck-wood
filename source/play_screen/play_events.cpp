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
  }
}

auto play_events::Increment(counter_type counterType) -> void
{
  switch( counterType )
  {
    case counter_type::enemies_destroyed:
      ++m_enemiesDestroyed;
      break;
    case counter_type::power_ups_collected:
      ++m_powerUpsCollected;
      break;
  }
}

auto play_events::Reset() -> void
{
  m_shot = m_explosion = false;
  m_enemiesDestroyed = m_bulletsDestroyed = m_powerUpsCollected = 0;
}

[[nodiscard]] auto play_events::Get(event_type eventType) const -> bool
{
  switch( eventType )
  {
    case event_type::shot:
      return m_shot;
    case event_type::explosion:
      return m_explosion;
    default:
      return false;
  }
}

[[nodiscard]] auto play_events::operator[](event_type eventType) const -> bool
{
  return Get(eventType);
}

[[nodiscard]] auto play_events::Get(counter_type counterType) const -> int
{
  switch( counterType )
  {
    case counter_type::enemies_destroyed:
      return m_enemiesDestroyed;
    case counter_type::power_ups_collected:
      return m_powerUpsCollected;
    default:
      return 0;
  }
}

[[nodiscard]] auto play_events::operator[](counter_type counterType) const -> int
{
  return Get(counterType);
}
