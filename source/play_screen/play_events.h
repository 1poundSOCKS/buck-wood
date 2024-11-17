#pragma once

class play_events
{

public:

  enum class event_type { basic };
  enum class basic_event_type { power_up_collected, time_bonus_collected, player_destroyed, enemy_destroyed, enemy_bullet_destroyed, player_shot, enemy_shot };

  using event_details = std::variant<basic_event_type>;
  using event_details_collection = std::vector<event_details>;

  static auto create(auto&&...args) -> void;
  static auto destroy() -> void;

  static auto reset() -> void;

  template <typename...Args>
  static [[nodiscard]] auto add(event_type eventType, Args...args) -> void;

  static [[nodiscard]] auto eventDetails() noexcept -> const event_details_collection&;

private:

  auto Reset() -> void;

private:

  inline static play_events* m_instance { nullptr };

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

template <typename...Args>
inline auto play_events::add(event_type eventType, Args...args) -> void
{
  m_instance->m_eventDetails.emplace_back(std::in_place_type<basic_event_type>, std::forward<Args>(args)...);
}

inline auto play_events::eventDetails() noexcept -> const event_details_collection &
{
  return m_instance->m_eventDetails;
}
