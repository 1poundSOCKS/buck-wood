#pragma once

class game_state
{

public:

  static auto create() noexcept -> void;
  static auto destroy() noexcept -> void;

  static [[nodiscard]] auto saved() noexcept -> bool;
  static [[nodiscard]] auto level_index() noexcept -> int;
  static [[nodiscard]] auto score() noexcept -> int;
  static [[nodiscard]] auto power_up_count() noexcept -> int;

  static auto set_level_index(int value) -> void;
  static auto set_score(int value) -> void;
  static auto set_power_up_count(int value) -> void;

private:

  [[nodiscard]] auto Saved() noexcept -> bool;

private:

  inline static game_state* m_instance { nullptr };

  int m_levelIndex { 0 };
  int m_score { 0 };
  int m_power_up_count { 0 };

};

inline auto game_state::create() noexcept -> void
{
  destroy();
  m_instance = new game_state();
}

inline auto game_state::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto game_state::saved() noexcept -> bool
{
  return m_instance->Saved();
}

inline [[nodiscard]] auto game_state::level_index() noexcept -> int
{
  return m_instance->m_levelIndex;
}

inline [[nodiscard]] auto game_state::score() noexcept -> int
{
  return m_instance->m_score;
}

inline [[nodiscard]] auto game_state::power_up_count() noexcept -> int
{
  return m_instance->m_power_up_count;
}

inline auto game_state::set_level_index(int value) -> void
{
  m_instance->m_levelIndex = value;
}

inline auto game_state::set_score(int value) -> void
{
  m_instance->m_score = value;
}

inline auto game_state::set_power_up_count(int value) -> void
{
  m_instance->m_power_up_count = value;
}
