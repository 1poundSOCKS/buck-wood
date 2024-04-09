#pragma once

class player_state
{

public:

  static auto create() noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get() -> player_state*;

  static [[nodiscard]] auto missile_count() noexcept -> int;

  static auto set_missile_count(int value) -> void;

private:

  inline static player_state* m_instance { nullptr };

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

inline [[nodiscard]] auto player_state::missile_count() noexcept -> int
{
  return m_instance->m_missileCount;
}

inline auto player_state::set_missile_count(int value) -> void
{
  m_instance->m_missileCount = value;
}
