#pragma once

#include "play_events.h"

class sound_effect_events
{

public:
  sound_effect_events()
  {
    for( auto&& eventDetail : play_events::eventDetails() )
    {
      std::visit([this](auto&& detail) { OnPlayEvent(detail); }, eventDetail);
    }
  }

  [[nodiscard]] auto Explosion() const noexcept -> bool;
  [[nodiscard]] auto ItemCollected() const noexcept -> bool;
  [[nodiscard]] auto PlayerShot() const noexcept -> bool;  

private:

  auto OnPlayEvent(play_events::basic_event_type basicEventType) noexcept -> void;
  auto OnPlayEvent(auto&& detail) noexcept -> void;

  bool m_explosion { false };
  bool m_itemCollected { false };
  bool m_playerShot { false };

};

auto sound_effect_events::OnPlayEvent(auto &&detail) noexcept -> void
{
}

inline auto sound_effect_events::Explosion() const noexcept -> bool
{
  return m_explosion;
}

inline auto sound_effect_events::ItemCollected() const noexcept -> bool
{
  return m_itemCollected;
}

inline auto sound_effect_events::PlayerShot() const noexcept -> bool
{
  return m_playerShot;
}

inline auto sound_effect_events::OnPlayEvent(play_events::basic_event_type basicEventType) noexcept -> void
{
  switch( basicEventType )
  {
    case play_events::basic_event_type::player_destroyed:
    case play_events::basic_event_type::enemy_destroyed:
    case play_events::basic_event_type::enemy_bullet_destroyed:
      m_explosion = true;
      break;
    case play_events::basic_event_type::power_up_collected:
    case play_events::basic_event_type::time_bonus_collected:
      m_itemCollected = true;
      break;
    case play_events::basic_event_type::player_shot:
      m_playerShot = true;
      break;
  }
}
