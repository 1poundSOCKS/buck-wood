#pragma once

class play_events
{

public:

  enum class event_type { shot, explosion };
  enum class counter_type { enemies_destroyed, bullets_destroyed, power_ups_collected };

  static auto create(auto&&...args) -> void;
  static auto destroy() -> void;

  static auto reset() -> void;

  static auto set(event_type eventType, bool value) -> void;
  static auto increment(counter_type counterType) -> void;

  static [[nodiscard]] auto get(event_type eventType) -> bool;
  static [[nodiscard]] auto get(counter_type counterType) -> int;

private:

  auto Reset() -> void;

  auto Set(event_type eventType, bool value) -> void;
  auto Increment(counter_type counterType) -> void;

  [[nodiscard]] auto Get(event_type eventType) const -> bool;
  [[nodiscard]] auto operator[](event_type eventType) const -> bool;

  [[nodiscard]] auto Get(counter_type counterType) const -> int;
  [[nodiscard]] auto operator[](counter_type counterType) const -> int;

private:

  inline static play_events* m_instance { nullptr };

  bool m_shot { false };
  bool m_explosion { false };
  int m_enemiesDestroyed { 0 };
  int m_bulletsDestroyed { 0 };
  int m_powerUpsCollected { 0 };

};

auto play_events::create(auto&&...args) -> void
{
  destroy();
  m_instance = new play_events { std::forward<decltype(args)>(args)... };
}

inline auto play_events::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto play_events::reset() -> void
{
  m_instance->Reset();
}

inline auto play_events::set(event_type eventType, bool value) -> void
{
  m_instance->Set(eventType, value);
}

inline auto play_events::increment(counter_type counterType) -> void
{
  m_instance->Increment(counterType);
}

inline [[nodiscard]] auto play_events::get(event_type eventType) -> bool
{
  return m_instance->Get(eventType);
}

inline [[nodiscard]] auto play_events::get(counter_type counterType) -> int
{
  return m_instance->Get(counterType);
}
