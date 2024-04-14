#pragma once

class player_state
{

public:

  enum class status { active, celebrating };

  static auto create() noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get() -> player_state*;

  static [[nodiscard]] auto get_status() noexcept -> status;
  static auto set_status(status value) -> void;

  static [[nodiscard]] auto missile_count() noexcept -> int;
  static auto set_missile_count(int value) -> void;
  static auto add_missiles(int value) -> int;
  static auto decrement_missile_count() -> int;

  static auto reset() -> void;

private:

  inline static player_state* m_instance { nullptr };

  status m_status { status::active };
  int m_missileCount { 0 };

};

inline auto player_state::create() noexcept -> void
{
  destroy();
  m_instance = new player_state();
}

inline auto player_state::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto player_state::get() -> player_state*
{
  return m_instance;
}

inline [[nodiscard]] auto player_state::get_status() noexcept -> status
{
  return m_instance->m_status;
}

inline auto player_state::set_status(status value) -> void
{
  m_instance->m_status = value;
}

inline [[nodiscard]] auto player_state::missile_count() noexcept -> int
{
  return m_instance->m_missileCount;
}

inline auto player_state::set_missile_count(int value) -> void
{
  m_instance->m_missileCount = value;
}

inline auto player_state::add_missiles(int value) -> int
{
  return m_instance->m_missileCount += value;
}

inline auto player_state::decrement_missile_count() -> int
{
  return --m_instance->m_missileCount;
}

inline auto player_state::reset() -> void
{
  m_instance->m_missileCount = 0;
}
