#pragma once

class play_events
{

public:

  enum class event_type { shot, explosion, basic };
  enum class counter_type { enemies_destroyed, power_ups_collected };
  enum class basic_event_type { power_up_collected, time_bonus_collected };

  using event_details = std::variant<basic_event_type>;
  using event_details_collection = std::vector<event_details>;

  static auto create(auto&&...args) -> void;
  static auto destroy() -> void;

  static auto reset() -> void;

  static auto set(event_type eventType, bool value) -> void;
  static auto increment(counter_type counterType) -> void;

  static [[nodiscard]] auto get(event_type eventType) -> bool;
  static [[nodiscard]] auto get(counter_type counterType) -> int;

  template <typename...Args>
  static [[nodiscard]] auto add(event_type eventType, Args...args) -> void;

  static [[nodiscard]] auto eventDetails() noexcept -> const event_details_collection&;

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

  event_details_collection m_eventDetails;

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

template <typename...Args>
inline auto play_events::add(event_type eventType, Args...args) -> void
{
  m_instance->m_eventDetails.emplace_back(std::in_place_type<basic_event_type>, std::forward<Args>(args)...);
}

inline auto play_events::eventDetails() noexcept -> const event_details_collection &
{
  return m_instance->m_eventDetails;
}
