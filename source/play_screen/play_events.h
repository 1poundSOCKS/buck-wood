#pragma once

class play_events
{

public:

  enum class event_type { shot, explosion, target_activated, power_up_collected };

  auto Set(event_type eventType, bool value) -> void;
  auto Reset() -> void;

  [[nodiscard]] auto Get(event_type eventType) const -> bool;
  [[nodiscard]] auto operator[](event_type eventType) const -> bool;

private:

  bool m_shot { false };
  bool m_explosion { false };
  bool m_targetActivated { false };
  bool m_powerUpCollected { false };

};
