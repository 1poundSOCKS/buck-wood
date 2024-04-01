#pragma once

class play_events
{

public:

  enum class event_type { shot, explosion, target_activated, power_up_collected };
  enum class counter_type { enemies_destroyed, bullets_destroyed };

  auto Set(event_type eventType, bool value) -> void;
  auto Increment(counter_type counterType) -> void;
  auto Reset() -> void;

  [[nodiscard]] auto Get(event_type eventType) const -> bool;
  [[nodiscard]] auto operator[](event_type eventType) const -> bool;

  [[nodiscard]] auto Get(counter_type counterType) const -> int;
  [[nodiscard]] auto operator[](counter_type counterType) const -> int;

private:

  bool m_shot { false };
  bool m_explosion { false };
  bool m_targetActivated { false };
  bool m_powerUpCollected { false };
  int m_enemiesDestroyed { 0 };
  int m_bulletsDestroyed { 0 };

};
